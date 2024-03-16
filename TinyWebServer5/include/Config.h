#ifndef _CONFIG_H_
#define _CONFIG_H_
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include <cstdint>
class Config
{
public:
	Config();
	~Config() = default;
	void parseArg(int argc, char* argv[]);

	bool logOpen;			//true为开启，false为close

	int maxQueueSize;		//=0为同步，>0为异步

	int maxSqlConn;			//最大数据库连接数

	int maxThreadNum;		//最大线程池线程数

	uint16_t port;

	bool optLinger;			//优雅关闭连接，false为关闭，true为开启
};
#endif
