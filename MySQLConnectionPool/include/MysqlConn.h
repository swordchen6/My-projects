#pragma once
#include<mysql/mysql.h>
#include<stdio.h>
#include<chrono>
#include<string>
class MysqlConn
{
public:
	MysqlConn();						//初始化数据库连接
	~MysqlConn();						//释放数据库连接
	bool connect(const char* host,
		const char* user, const char* passwd, const char* db, 
		unsigned int port=3306,const char* unix_socket=nullptr, 
		unsigned long client_flag=0);	//连接数据库
	bool update(const char* query);		//更新数据库 : insert，update， delete
	bool query(const char* query);		//查询数据库
	bool nextRecond();					//遍历查询得到的结果集
	std::string getValue(int index);	//得到结果集中的字段值
	bool transaction();					//事务操作
	bool commit();						//提交事务
	bool rollBack();					//回滚事务
	void refreshAliveTime();			//刷新起始的空闲时间点
	long long getAliveTime();			//计算空闲时间
private:
	void freeResult();
	MYSQL* conn;
	MYSQL_RES* result;
	MYSQL_ROW row;
	std::chrono::steady_clock::time_point aliveTime;
};