#ifndef _CONN_POOL_
#define _CONN_POOL_
#include<queue>
#include<memory>
#include"MysqlConn.h"
#include"Locker.h"
class ConnPool
{
public:
	~ConnPool();
	static ConnPool* singleton();
	void init(int pMaxConn, const char* pHost, const char* pUser, const char* pPasswd, 
		const char* pDb,unsigned int pPort, const char* pUnixsocket, unsigned long pClientFlag);
	std::shared_ptr<MysqlConn> getConn();
	bool releaseConn(MysqlConn* conn);
private:
	ConnPool() = default;
	ConnPool(const ConnPool&) = delete;
	ConnPool& operator=(const ConnPool&) = delete;

	bool shutDown;

	std::queue<MysqlConn*> connQueue;
	long unsigned int maxConn;

	Cond getCond;
	Cond releaseCond;

	Mutex mutex;
	
	const char* host;
	const char* user;
	const char* passwd;
	const char* db;
	unsigned int port; 
	const char* unixSocket; 
	unsigned long clientFlag;
};
#endif

