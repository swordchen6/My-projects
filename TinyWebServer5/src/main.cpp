#include<stdio.h>
#include<assert.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<signal.h>
#include"Log.h"
#include"ConnPool.h"
#include"Timer.h"
#include"HttpConn.h"
#include"Config.h"
#include"WebServer.h"
int main(int argc,char* argv[])
{
    Config config;
    config.parseArg(argc, argv);

#if 1
    WebServer server(config.port,config.optLinger);
//#if 0
    server.logStart(config.logOpen, config.maxQueueSize, "WebServer");
    server.connPoolStart(config.maxSqlConn, "192.168.188.88", 
        "chen", "123456", "yourdb", 3306, NULL, 0);
    server.threadPoolStart(config.maxThreadNum);
    server.timerStart();
    server.httpStart("/home/cdz/projects/TinyWebServer5/root");
    printf("listen start!\n");
    server.eventListen();
    server.eventLoop();
#endif
}