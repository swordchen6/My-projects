#include "ConnPool.h"

ConnPool::~ConnPool()
{
	mutex.lock();
	shutDown = true;
	while (!connQueue.empty())
	{
		MysqlConn* conn = connQueue.front();
		connQueue.pop();
		delete conn;
	}
	mutex.unlock();
	getCond.broadcast();
	releaseCond.broadcast();
}

ConnPool* ConnPool::singleton()
{
	static ConnPool pool;
	return &pool;
}

void ConnPool::init(int pMaxConn, const char* pHost, const char* pUser,
	const char* pPasswd, const char* pDb, unsigned int pPort, const char* pUnixsocket, 
	unsigned long pClientFlag)
{
	shutDown = false;
	maxConn = pMaxConn;
	host = pHost;
	user = pUser;
	passwd = pPasswd;
	db = pDb;
	port = pPort;
	unixSocket = pUnixsocket;
	clientFlag = pClientFlag;
	for (long unsigned i = 0; i < maxConn; i++)
	{
		MysqlConn* conn = new MysqlConn;
		if (!conn->connect(host, user, passwd, db, port, unixSocket, clientFlag))
		{
			throw std::exception();
		}
		connQueue.push(conn);
	}
}

std::shared_ptr<MysqlConn> ConnPool::getConn()
{
	mutex.lock();
	while (!shutDown&&connQueue.empty())
	{
		if (!getCond.wait(mutex.get()))
		{
			mutex.unlock();
			return NULL;
		}
	}
	if (shutDown)
	{
		mutex.unlock();
		return NULL;
	}
	MysqlConn* conn = connQueue.front();
	connQueue.pop();
	mutex.unlock();
	releaseCond.signal();
	return std::shared_ptr<MysqlConn>(conn,
		[](MysqlConn* conn) {ConnPool::singleton()->releaseConn(conn); });
}

bool ConnPool::releaseConn(MysqlConn* conn)
{
	mutex.lock();
	while (!shutDown && connQueue.size() >= maxConn)
	{
		if (!releaseCond.wait(mutex.get()))
		{
			mutex.unlock();
			return false;
		}
	}
	if (shutDown)
	{
		mutex.unlock();
		return false;
	}
	connQueue.push(conn);
	mutex.unlock();
	getCond.signal();
	return true;
}
