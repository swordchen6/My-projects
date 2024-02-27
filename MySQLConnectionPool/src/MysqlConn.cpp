#include "MysqlConn.h"
#include<iostream>

MysqlConn::MysqlConn()
{
	conn = mysql_init(nullptr);
	if (conn == nullptr)
	{
		printf("mysql_init error!\n");
	}
	result = nullptr;
	if (mysql_set_character_set(conn, "utf8")!=0)
	{
		printf("mysql_set_character_set error!\n");
	}
}

MysqlConn::~MysqlConn()
{
	if (conn)
	{
		mysql_close(conn);
	}
	freeResult();
}

bool MysqlConn::connect(const char* host, const char* user, 
	const char* passwd, const char* db, unsigned int port, 
	const char* unix_socket, unsigned long client_flag)
{
	mysql_real_connect(conn, host, user, passwd, db, 
		port, unix_socket, client_flag);
	return conn != nullptr;
}

bool MysqlConn::update(const char* query)
{
	int ret = mysql_query(conn, query);
	return ret == 0;
}

bool MysqlConn::query(const char* query)
{
	freeResult();
	int ret = mysql_query(conn, query);
	if (ret != 0)
		std::cout << "mysql_query error!:"<<mysql_error(conn) << std::endl;
	result=mysql_store_result(conn);
	return ret == 0&&result;
}

bool MysqlConn::nextRecond()
{
	if (result == nullptr)
		return false;
	row = mysql_fetch_row(result);
	if (row == nullptr)
		return false;
	return true;
}
std::string MysqlConn::getValue(int index)
{
	unsigned int columnCount = mysql_num_fields(result);
	if (index >= (int)columnCount || index < 0)
		return nullptr;
	char* val = row[index];
	unsigned long length = mysql_fetch_lengths(result)[index];
	return std::string(val,length);
}

bool MysqlConn::transaction()
{
	return mysql_autocommit(conn, false)==0;
}

bool MysqlConn::commit()
{
	return mysql_commit(conn)==0;
}

bool MysqlConn::rollBack()
{
	return mysql_rollback(conn)==0;
}

void MysqlConn::refreshAliveTime()
{
	aliveTime = std::chrono::steady_clock::now();
}

long long MysqlConn::getAliveTime()
{
	std::chrono::nanoseconds res1= std::chrono::steady_clock::now()-aliveTime;
	std::chrono::milliseconds res2 = std::chrono::duration_cast
		<std::chrono::milliseconds>(res1);
	return res2.count();
}

void MysqlConn::freeResult()
{
	if (result)
	{
		mysql_free_result(result);
	}
}

