#ifndef _WEB_SERVER_H_
#define _WEB_SERVER_H_
#include<arpa/inet.h>
#include<sys/socket.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/epoll.h>
#include<errno.h>
#include<signal.h>
#include"Timer.h"
#include"ThreadPool.hpp"
#include"Log.h"
class WebServer
{
public:
	WebServer(uint16_t pPort,bool pOptLinger);
	void logStart(bool logOpen, int maxQueueSize, const char* logName);
	void connPoolStart(int pMaxConn, const char* pHost, const char* pUser, const char* pPasswd,
		const char* pDb, unsigned int pPort, const char* pUnixsocket, unsigned long pClientFlag);
	void threadPoolStart(int pMaxThread);

	void timerStart();

	void httpStart(const char* pRoot);

	void eventListen();
	void eventLoop();

	const int static MAX_FD = 16384;
	const int static MAX_EVENT_NUMBER = 16384;

	int listenFd;
	int epollFd;
	uint16_t port;
	bool optLinger;
	static int pipeFd[2];
	int userCount;
	epoll_event events[MAX_EVENT_NUMBER];
	Log* log;
	HttpConn* https;
	UserData* datas;
	TimerHeap* timers;
	ThreadPool<HttpConn>* threadPool;
	ConnPool* connPool;
private:
	void resetTime();
	int setNonBlock(int fd);
	void addFd(int fd, bool oneShot, bool trigMode);
	void modFd(int fd, int ev, bool trigMode);
	void removeFd(int fd);
	static void sigHandler(int sig);
	void addSig(int sig, void(handler)(int), bool restart=true);

	bool dealWithClient();
	bool dealWithError(int sockfd);
	bool dealWithSignal(bool& timeOut, bool& serverStop);
	bool dealWithRead(int sockfd);
	bool dealWithWrite(int sockfd);
};

#endif

