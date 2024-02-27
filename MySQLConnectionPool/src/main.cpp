#include <iostream>                                                                                                          
#include"ConnectionPool.h"
using namespace std;
using namespace chrono;
void testMysqlConn()
{
    MysqlConn* conn = new MysqlConn;
    conn->connect("192.168.188.88", "chen", "123456", "test");
    if (conn == nullptr)
        std::cout << "connect error" << std::endl;
    conn->query("select * from student");
    conn->update("insert into student(name,age) values(\"zzz\",91)");
    while (conn->nextRecond())
    {
        std::cout << conn->getValue(0) << " " << conn->getValue(1) << " " << conn->getValue(2) << std::endl;
    }
}
void testcJSON()
{
    ConnectionPool* pool = ConnectionPool::getInstance();
    pool->printMess();
}
void testConnectionPool()
{
    ConnectionPool* pool = ConnectionPool::getInstance();
    std::shared_ptr<MysqlConn> conn = pool->getConn();
    conn->query("select * from student");
    while (conn->nextRecond())
    {
        std::cout << conn->getValue(0) << " " << conn->getValue(1) << " " << conn->getValue(2) << std::endl;
    }
}
//1.单线程：使用/不使用连接池
//2.多线程：使用/不适用连接池
void* op1(void* n)
{
    int num = *(int*)n;
    for (int i = 0; i < num; i++)
    {
        MysqlConn conn;
        conn.connect("192.168.188.88", "chen", "123456", "test");
        conn.update("insert into student(name,age) values(\"tom\",100)");
    }
    MysqlConn conn;
    conn.connect("192.168.188.88", "chen", "123456", "test");
    conn.update("delete from student where name=\"tom\"");
    return nullptr;
}
void* op2(void* n)
{
    int num = *(int*)n;
    ConnectionPool* pool = ConnectionPool::getInstance();
    for (int i = 0; i < num; i++)
    {
        shared_ptr<MysqlConn> connPtr = pool->getConn();
        connPtr->update("insert into student(name,age) values(\"tom\",100)");
    }
    shared_ptr<MysqlConn> connPtr = pool->getConn();
    connPtr->update("delete from student where name=\"tom\"");
    return nullptr;
}
void test1()
{
    int* num = new int(5000);
    steady_clock::time_point begin = steady_clock::now();
    op1(num);
    steady_clock::time_point end = steady_clock::now();
    cout << "单线程不使用连接池耗费时间:" <<
        duration_cast<milliseconds>(end - begin).count()
        << "ms" << endl;
    delete num;
}
void test2()
{
    int* num = new int(5000);
    steady_clock::time_point begin = steady_clock::now();
    op2(num);
    steady_clock::time_point end = steady_clock::now();
    cout << "单线程使用连接池耗费时间:" <<
        duration_cast<milliseconds>(end - begin).count()
        << "ms" << endl;
    delete num;
}
void test3()
{
    pthread_t threads[5] = {};
    int* num = new int(1000);
    steady_clock::time_point begin = steady_clock::now();
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&threads[i], nullptr, op1, num);
        pthread_join(threads[i], nullptr);
    }
    steady_clock::time_point end = steady_clock::now();
    cout << "多线程使用连接池耗费时间:" <<
        duration_cast<milliseconds>(end - begin).count()
        << "ms" << endl;
    delete num;
}
void test4()
{
    pthread_t threads[5] = {};
    int* num = new int(1000);
    steady_clock::time_point begin = steady_clock::now();
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&threads[i], nullptr, op2, num);
        pthread_join(threads[i], nullptr);
    }
    steady_clock::time_point end = steady_clock::now();
    cout << "多线程使用连接池耗费时间:" <<
        duration_cast<milliseconds>(end - begin).count()
        << "ms" << endl;
    delete num;
}
int main()
{
#ifdef TEST1
    test1();
    test2();
    //得到结果
    //单线程不使用连接池耗费时间:51509ms
    //单线程使用连接池耗费时间:5572ms
    //省去将近十分之九的时间
#endif // TEST1
#ifdef TEST2
    test3();
    test4();
    //单线程使用连接池耗费时间:51843ms
    //单线程使用连接池耗费时间:6227ms
#endif // TEST2

    return 0;
}