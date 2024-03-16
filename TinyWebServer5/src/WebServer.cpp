#include "WebServer.h"
int WebServer::pipeFd[2];
void WebServer::resetTime()
{
	std::vector<int> ans=timers->tick();
	for (long unsigned int i = 0; i < ans.size(); i++)
	{
		char a[36];
		inet_ntop(AF_INET, &datas[ans[i]].addr.sin_addr.s_addr, a, 36);
		LOG_INFO("--WebServer: Client %d is timeout,address is %s\n", ans[i],a);
		removeFd(ans[i]);
		userCount--;
		https[ans[i]].reset();
		https[ans[i]].state = HttpConn::WAIT_READ;
		https[ans[i]].cancelTimer = false;
		https[ans[i]].improv = false;
	}
	alarm(timers->TIMESLOT);
}

int WebServer::setNonBlock(int fd)
{
	int oldOption = fcntl(fd, F_GETFL);
	int newOption = oldOption | O_NONBLOCK;
	fcntl(fd, F_SETFL, newOption);
	return oldOption;
}

void WebServer::addFd(int fd, bool oneShot, bool trigMode)
{
	epoll_event event;
	event.data.fd = fd;

	if (trigMode == true)
		event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
	else
		event.events = EPOLLIN | EPOLLRDHUP;

	if (oneShot)
		event.events |= EPOLLONESHOT;
	epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event);
	setNonBlock(fd);
}

void WebServer::removeFd(int fd)
{
	epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, 0);
	close(fd);
}

void WebServer::sigHandler(int sig)
{
	//为保证函数的可重入性，保留原来的errno
	int save_errno = errno;
	int msg = sig;
	send(pipeFd[1], (char*)&msg, 1, 0);
	errno = save_errno;
}

void WebServer::modFd(int fd, int ev, bool trigMode)
{
	epoll_event event;
	event.data.fd = fd;

	if (trigMode==true)
		event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
	else
		event.events = ev | EPOLLONESHOT | EPOLLRDHUP;

	epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, &event);
}

void WebServer::addSig(int sig, void(handler)(int), bool restart)
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handler;
	if (restart)
		sa.sa_flags |= SA_RESTART;
	sigfillset(&sa.sa_mask);
	assert(sigaction(sig, &sa, NULL) != -1);
}

bool WebServer::dealWithClient()
{
	struct sockaddr_in clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	int connfd = accept(listenFd, (struct sockaddr*)&clientAddr, &clientAddrLen);
	if (connfd < 0)
	{
		return false;
	}
	if (userCount >= MAX_FD)
	{
		send(connfd, "Internal server busy", strlen("Internal server busy"),0);
		close(connfd);
		return false;
	}
	https[connfd].fd = connfd;
	timers->setTimer(connfd, clientAddr, datas + connfd);
	addFd(connfd, true, true);
	char a[36];
	inet_ntop(AF_INET, &datas[connfd].addr.sin_addr.s_addr, a, 36);
	LOG_INFO("--WebServer: Client %d is listening,address is %s\n", connfd, a);
	userCount++;
	return true;
}

bool WebServer::dealWithError(int sockfd)
{
	timers->delTimer(datas[sockfd].index);
	removeFd(sockfd);
	userCount--;
	return true;
}

bool WebServer::dealWithSignal(bool& timeOut, bool& serverStop)
{
	char signals[1024];
	ssize_t ret = recv(pipeFd[0], signals, sizeof(signals), 0);
	if (ret == -1)
	{
		return false;
	}
	else if (ret == 0)
	{
		return false;
	}
	else
	{
		for (int i = 0; i < ret; ++i)
		{
			switch (signals[i])
			{
			case SIGALRM:
			{
				timeOut = true;
				break;
			}
			case SIGTERM:
			{
				serverStop = true;
				break;
			}
			}
		}
	}
	return true;
}

bool WebServer::dealWithRead(int sockfd)
{
	if (https[sockfd].state != HttpConn::WAIT_READ)
	{
		return false;
	}
	timers->adjustTimer(datas[sockfd].index);
	threadPool->append(https + sockfd);
	LOG_INFO("--WebServer: Client %d: dealWithRead() start\n", sockfd);
	while (true)
	{
		if (https[sockfd].improv == true)
		{	
			if (https[sockfd].cancelTimer)
			{
				dealWithError(sockfd);
				https[sockfd].cancelTimer = false;
			}
			else if (https[sockfd].state == HttpConn::WAIT_READ)
			{
				modFd(sockfd, EPOLLIN, true);
			}
			else if (https[sockfd].state == HttpConn::WAIT_WRITE)
			{
				modFd(sockfd, EPOLLOUT, true);
			}
			https[sockfd].improv = false;
			break;
		}
	}
	return true;
}
bool WebServer::dealWithWrite(int sockfd)
{
	if (https[sockfd].state != HttpConn::WAIT_WRITE)
	{
		return false;
	}
	timers->adjustTimer(datas[sockfd].index);
	threadPool->append(https + sockfd);
	LOG_INFO("--WebServer: Client %d: dealWithWrite() start\n", sockfd);
	while (true)
	{
		if (https[sockfd].improv==true)
		{
			if (https[sockfd].cancelTimer)
			{
				dealWithError(sockfd);
				https[sockfd].cancelTimer = false;
			}
			else if (https[sockfd].state == HttpConn::WAIT_READ)
			{
				modFd(sockfd, EPOLLIN, true);
			}
			else if (https[sockfd].state == HttpConn::WAIT_WRITE)
			{
				modFd(sockfd, EPOLLOUT, true);
			}
			https[sockfd].improv = false;
			break;
		}
	}
	return true;
}


WebServer::WebServer(uint16_t pPort, bool pOptLinger)
{
	port = pPort;
	optLinger = pOptLinger;
}

void WebServer::logStart(bool logOpen,  int maxQueueSize,const char* logName)
{
	if (logOpen)
	{
		log = Log::singleton();
		if (maxQueueSize>0)
		{
			log->init(logName, 800000, maxQueueSize);
		}
		else
		{
			log->init(logName, 800000, 0);
		}
	}
}

void WebServer::connPoolStart(int pMaxConn, const char* pHost, const char* pUser,
	const char* pPasswd, const char* pDb, unsigned int pPort, const char* pUnixsocket, 
	unsigned long pClientFlag)
{
	connPool = ConnPool::singleton();
	connPool->init(pMaxConn, pHost, pUser, pPasswd, pDb, pPort, pUnixsocket, pClientFlag);
}

void WebServer::threadPoolStart(int pMaxThread)
{
	threadPool = ThreadPool<HttpConn>::singleton();
	threadPool->init(pMaxThread, 10000);
}

void WebServer::timerStart()
{
	timers = new TimerHeap(MAX_FD);
	if (!timers)
	{
		throw std::exception();
	}
	datas = new UserData[MAX_FD];
	if (!datas)
	{
		throw std::exception();
	}
}

void WebServer::httpStart(const char* pRoot)
{
	https = new HttpConn[MAX_FD];
	if (!https)
	{
		throw std::exception();
	}
	strcpy(HttpConn::root, pRoot);
}


void WebServer::eventListen()
{
	listenFd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenFd >= 0);
	printf("listen fd is %d\n", listenFd);

	if (optLinger)
	{
		struct linger tmp = { 1, 1 };
		setsockopt(listenFd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
	}
	else
	{
		struct linger tmp = { 0, 1 };
		setsockopt(listenFd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
	}

	int flag = 1;
	setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(port);

	int ret = bind(listenFd, (struct sockaddr*)&addr, sizeof(addr));
	assert(ret >= 0);

	ret = listen(listenFd, 128);
	assert(ret >= 0);

	epollFd = epoll_create(5);
	assert(epollFd >= 0);

	addFd(listenFd, false, false);			

	ret = socketpair(AF_UNIX, SOCK_STREAM, 0, pipeFd);
	assert(ret >= 0);
	setNonBlock(pipeFd[1]);
	addFd(pipeFd[0], false, false);

	addSig(SIGPIPE, SIG_IGN);
	addSig(SIGALRM, sigHandler, false);
	addSig(SIGTERM, sigHandler, false);

	alarm(timers->TIMESLOT);
}

void WebServer::eventLoop()
{
	bool timeOut = false;
	bool serverStop = false;

	while (!serverStop)
	{
		int num = epoll_wait(epollFd, events, MAX_EVENT_NUMBER, -1);
		if (num < 0 && errno != EINTR)
		{
			break;
		}
		for (int i = 0; i < num; i++)
		{
			int sockfd = events[i].data.fd;
			if (sockfd == listenFd)
			{
				bool flag = dealWithClient();
				if (flag == false)
					continue;
			}
			else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
			{
				dealWithError(sockfd);
			}
			else if ((sockfd == pipeFd[0]) && (events[i].events & EPOLLIN))
			{
				bool flag = dealWithSignal(timeOut, serverStop);
				if (flag == false)
				{
					continue;
				}
			}
			else if (events[i].events & EPOLLIN)
			{
				dealWithRead(sockfd);
			}
			else if (events[i].events & EPOLLOUT)
			{
				dealWithWrite(sockfd);
			}
		}
		if (timeOut)
		{
			resetTime();
			timeOut = false;
		}
	}
}
