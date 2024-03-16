#include "MysqlConn.h"

MysqlConn::MysqlConn()
{
	conn = mysql_init(NULL);
	if (conn == NULL)
	{
		throw std::exception();
	}
}

MysqlConn::~MysqlConn()
{
	mysql_close(conn);
}

bool MysqlConn::connect(const char* pHost, const char* pUser, const char* pPasswd, const char* pDb,
	unsigned int pPort, const char* pUnixSocket, unsigned long pClientFlag)
{
	return mysql_real_connect(conn, pHost, pUser, pPasswd, pDb, pPort, 
		pUnixSocket, pClientFlag);
}

MYSQL* MysqlConn::get()
{
	return conn;
}
