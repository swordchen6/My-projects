#ifndef _MYSQL_CONN_H_
#define _MYSQL_CONN_H_
#include<mysql/mysql.h>
#include<exception>
class MysqlConn
{
public:
	MysqlConn();
	~MysqlConn();
	bool connect(const char* pHost, const char* pUser, const char* pPasswd, const char* pDb, 
		unsigned int pPort,const char* pUnixSocket, unsigned long pClientFlag);
	MYSQL* get();
private:
	MYSQL* conn;
};
#endif

