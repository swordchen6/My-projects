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

	bool logOpen;			//trueΪ������falseΪclose

	int maxQueueSize;		//=0Ϊͬ����>0Ϊ�첽

	int maxSqlConn;			//������ݿ�������

	int maxThreadNum;		//����̳߳��߳���

	uint16_t port;

	bool optLinger;			//���Źر����ӣ�falseΪ�رգ�trueΪ����
};
#endif
