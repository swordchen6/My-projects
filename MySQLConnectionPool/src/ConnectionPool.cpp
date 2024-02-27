#include "ConnectionPool.h"                                                                                                  

ConnectionPool* ConnectionPool::getInstance()
{
    static ConnectionPool pool;
    return &pool;
}

std::shared_ptr<MysqlConn> ConnectionPool::getConn()
{
    pthread_mutex_lock(&queueMutex);
    int ret = 0;
    std::shared_ptr<MysqlConn> conn = nullptr;
    while (connQueue.empty())//当前没有连接就等一等，等不到返回nullptr
    {
        struct timespec tmsp = {};
        tmsp.tv_nsec = 0;
        tmsp.tv_sec = time(nullptr) + timeOut / 1000;
        ret = pthread_cond_timedwait(&reduceCond, &queueMutex, &tmsp);
    }
    if (ret == 0)
    {
        conn = std::shared_ptr<MysqlConn>(connQueue.front(),
            [](MysqlConn* conn) {
                ConnectionPool* pool = ConnectionPool::getInstance();
                conn->refreshAliveTime();
                if (pool)
                    pool->receiveConn(conn);
                else
                    delete conn;
            });
        connQueue.pop();
    }
    pthread_mutex_unlock(&queueMutex);
    pthread_cond_signal(&produceCond);
    return conn;
}

ConnectionPool::ConnectionPool()
{
    shutDown = false;
    if (!parseJsonFile())
        return;
    if (pthread_mutex_init(&queueMutex, nullptr) != 0 ||
        pthread_cond_init(&produceCond, nullptr) != 0 ||
        pthread_cond_init(&reduceCond, nullptr) != 0)
    {
        std::cout << "lock or cond init error!" << std::endl;
        return;
    }
    for (int i = 0; i < minSize; i++)
    {
        addConn();
    }
    currSize = minSize;
    pthread_create(&producer, nullptr, produceConn, nullptr);
    pthread_detach(producer);
    pthread_create(&reducer, nullptr, recycleConn, nullptr);
    pthread_detach(reducer);
}

bool ConnectionPool::parseJsonFile()
{
    // 打开文件
    FILE* file = nullptr;
    file = fopen("dbconf.json", "r");
    if (file == nullptr) {
        std::cout << "fopen error!" << std::endl;
        return false;
    }

    // 获得文件大小
    struct stat statbuf;
    stat("dbconf.json", &statbuf);
    off_t fileSize = statbuf.st_size;

    // 分配符合文件大小的内存
    char* jsonStr = new char[fileSize + 1];
    memset(jsonStr, 0, fileSize + 1);

    // 读取文件中的json字符串
    size_t size = fread(jsonStr, sizeof(char), fileSize, file);
    if (size == 0) {
        std::cout << "fread error!" << std::endl;
        fclose(file);
        return false;
    }
    fclose(file);
    root = cJSON_Parse(jsonStr);
    if (!root)
    {
        std::cout << "cJSON_Parse error!" << std::endl;
        return false;
    }
    cJSON* m_host = cJSON_GetObjectItem(root, "host");
    cJSON* m_user = cJSON_GetObjectItem(root, "user");
    cJSON* m_passwd = cJSON_GetObjectItem(root, "passwd");
    cJSON* m_dbname = cJSON_GetObjectItem(root, "dbname");
    cJSON* m_port = cJSON_GetObjectItem(root, "port");
    cJSON* m_minSize = cJSON_GetObjectItem(root, "minSize");
    cJSON* m_maxSize = cJSON_GetObjectItem(root, "maxSize");
    cJSON* m_timeOut = cJSON_GetObjectItem(root, "timeOut");
    cJSON* m_idleTime = cJSON_GetObjectItem(root, "idleTime");
    host = m_host->valuestring;
    user = m_user->valuestring;
    passwd = m_passwd->valuestring;
    dbname = m_dbname->valuestring;
    port = m_port->valueint;
    minSize = m_minSize->valueint;
    maxSize = m_maxSize->valueint;
    timeOut = m_timeOut->valueint;
    idleTime = m_idleTime->valueint;
    delete[]jsonStr;
    return true;
}

void ConnectionPool::addConn()
{
    MysqlConn* conn = new MysqlConn;
    conn->connect(host, user, passwd, dbname, port);
    if (conn)
    {
        conn->refreshAliveTime();
        connQueue.push(conn);
        currSize++;
    }
}

void ConnectionPool::receiveConn(MysqlConn * conn)
{
    pthread_mutex_lock(&queueMutex);
    ConnectionPool* pool = this->getInstance();
    pool->connQueue.push(conn);
    pthread_mutex_unlock(&queueMutex);
    pthread_cond_signal(&pool->reduceCond);
}

ConnectionPool::~ConnectionPool()
{
    shutDown = true;
    pthread_mutex_lock(&queueMutex);
    while (!connQueue.empty())
    {
        MysqlConn* conn = connQueue.front();
        connQueue.pop();
        delete conn;
    }
    pthread_mutex_unlock(&queueMutex);
    pthread_cond_signal(&produceCond);
    pthread_mutex_destroy(&queueMutex);
    pthread_cond_destroy(&produceCond);
    pthread_cond_destroy(&reduceCond);
    cJSON_Delete(root);
}

void ConnectionPool::printMess()
{
    std::cout << "host: " << host << std::endl;
    std::cout << "user: " << user << std::endl;
    std::cout << "passwd: " << passwd << std::endl;
    std::cout << "dbname: " << dbname << std::endl;
    std::cout << "port: " << port << std::endl;

    std::cout << "minSize: " << minSize << std::endl;
    std::cout << "maxSize: " << maxSize << std::endl;
    std::cout << "timeOut: " << timeOut << std::endl;
    std::cout << "idleTime: " << idleTime << std::endl;
}
//需要产生连接的条件分析：
//currSize<maxSize
//connQueue.size()<minSize
//两个条件是并关系
void* ConnectionPool::produceConn(void* arg)
{
    ConnectionPool* pool = ConnectionPool::getInstance();
    while (!pool->shutDown)
    {
        pthread_mutex_lock(&pool->queueMutex);
        while (pool->currSize >= pool->maxSize ||
            pool->connQueue.size() >= pool->minSize)
        {
            pthread_cond_wait(&pool->produceCond, &pool->queueMutex);
            if (pool->shutDown)
            {
                pthread_mutex_unlock(&pool->queueMutex);
                break;
            }
        }
        pool->addConn();
        pthread_mutex_unlock(&pool->queueMutex);
        pthread_cond_signal(&pool->reduceCond);
    }
    return nullptr;
}
//需要销毁连接的条件分析：
//currSize>maxSize
//connQueue.size()>minSize
//两个条件是并关系
void* ConnectionPool::recycleConn(void* arg)
{
    ConnectionPool* pool = ConnectionPool::getInstance();
    while (!pool->shutDown)
    {
        pthread_mutex_lock(&pool->queueMutex);
        while (pool->currSize > pool->maxSize &&
            pool->connQueue.size() > pool->minSize)
        {
            MysqlConn* conn = pool->connQueue.front();
            if (conn->getAliveTime() >= pool->idleTime)
            {
                pool->connQueue.pop();
                delete conn;
                pool->currSize--;
            }
        }
        pthread_mutex_unlock(&pool->queueMutex);
    }
    return nullptr;
}