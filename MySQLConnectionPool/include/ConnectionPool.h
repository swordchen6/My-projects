#pragma once
#include<queue>
#include<pthread.h>
#include"cJSON.h"
#include"MysqlConn.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<string.h>
#include<memory>
#include<iostream>
class ConnectionPool
{
public:
	static ConnectionPool* getInstance();//单例模式
	ConnectionPool(const ConnectionPool& obj)=delete;
	ConnectionPool& operator=(const ConnectionPool& obj)=delete;
	std::shared_ptr<MysqlConn> getConn();//拿连接
	void receiveConn(MysqlConn* conn);	 //放回线程
	~ConnectionPool();
	void printMess();
private:
	ConnectionPool();					//构造函数
	bool parseJsonFile();				//解析json文件
	void addConn();						//添加连接,线程非安全
	static void* produceConn(void* arg);//生产连接线程
	static void* recycleConn(void* arg);//回收连接线程
	std::queue<MysqlConn*> connQueue;	//连接队列
	char* host;							//数据库服务器ip地址
	char* user;							//数据库用户名
	char* passwd;						//用户名密码
	char* dbname;						//数据库名称
	unsigned int port;					//连接数据库服务器的端口
	
	pthread_t producer;					//创建连接的线程号
	pthread_t reducer;					//销毁连接的线程号

	cJSON* root;						//json文件的根节点

	int currSize;						//分配出去的连接+队列的连接即总连接数
	int minSize;						//最小连接数
	int	maxSize;						//最大连接数
	int timeOut;						//超时时长,毫秒单位
	int idleTime;						//最大空闲时长，毫秒单位
	bool shutDown;						//是否关闭

	pthread_mutex_t queueMutex;			//连接队列的互斥锁
	pthread_cond_t produceCond;			//连接生产条件变量，提示可以生产
	pthread_cond_t reduceCond;			//连接销毁条件变量，提示可以销毁
};

