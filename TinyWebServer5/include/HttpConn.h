#ifndef _HTTP_CONN_H_
#define _HTTP_CONN_H_
#include<unistd.h>
#include<sys/socket.h>
#include<errno.h>
#include<sys/uio.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include"ConnPool.h"
#include"Log.h"
//大致思路
//reset函数的必要性，因为一开始就new出了HttpConn的数组，那么不好单独释放一个元素空间
//即使可以这样做也浪费了时间。
//
//read函数
    //进入该函数只有state为WAIT_READ才可执行
    //WAIT_READ表示未被处理，或未被处理完
    //推出该函数读完或者读一半返回true，读失败返回false
    //读失败返回false，线程池的线程会标记为可以断开该连接，然后从定时器中剔除
    //读完或者读一半进入下一步的process函数
//process函数 
    //parseRead函数利用主从状态机
    //  从状态机可以剔除不完整的报文，即read函数读一半的情况，这时parseLine函数返回LINE_OPEN
    //      然后不满足主状态机的循环条件，退出主状态机，parseRead函数返回NO_REQUEST
    //      然后直接从process函数退出，这时state仍为WAIT_READ，这时要继续监听读事件
    //  主状态机从接收的报文解析数据，除NO_REQUEST,其他情况都可到parseWrite处理
    //parseWrite函数对parseRead函数的返回值做对应的处理，如果写缓存足够写入的内容返回true
    //  否则返回false。返回true,state为WAIT_WRITE，这时应监听写事件
    //  返回false，state为PROCESS_ERROR，这时断开连接，然后从定时器中剔除
//write函数
    //只有state为WAIT_WRITE才可进入
    //如果只能写一半，返回true，继续监听写事件
    //如果写失败，返回false，可以断开该连接，然后从定时器中剔除
    //如果写完，linger为true，保持连接，返回true
    //linger为false，断开连接，然后从定时器中剔除，返回false，

class HttpConn
{
public:
    static const int FILE_NAME_LEN = 200;
    static const int READ_BUFFER_SIZE = 2048;
    static const int WRITE_BUFFER_SIZE = 1024;
    static const int ROOT_PATH_LEN = 128;

    enum PARSE_STATE
    {
        NO_REQUEST,GET_REQUEST,BAD_REQUEST, INTERNAL_ERROR,
        NO_RESOURCE,FORBIDDEN_REQUEST,FILE_REQUEST,CLOSED_CONNECTION
    };

    enum METHOD
    {
        GET,POST,HEAD,PUT, DELETE,
        TRACE,OPTIONS,CONNECT,PATCH
    };

    enum UPPER_STATE
    {
        CHECK_STATE_REQUESTLINE,
        CHECK_STATE_HEADER,
        CHECK_STATE_CONTENT
    };

    enum BOTTOM_STATE
    {
        LINE_OK,LINE_BAD,LINE_OPEN
    };

    enum PROCESS_RET{WAIT_READ,PROCESS_ERROR,WAIT_WRITE};

    HttpConn();
    ~HttpConn() = default;
    bool read();//成功继续下一步，失败删除定时器，关闭连接
    bool write();
    PROCESS_RET process();
    void reset();

    int fd;                             //由WebServer的dealwithclient赋值
    static char root[ROOT_PATH_LEN];    //由WebServer的httpstart赋值
    PROCESS_RET state;
    bool improv;                        //是否被处理过
    bool cancelTimer;                   //是否需要删除定时器
private:
    BOTTOM_STATE parseLine();
    PARSE_STATE parseRead();
    PARSE_STATE parseRequestLine(char* text);
    PARSE_STATE parseHeader(char* text);
    PARSE_STATE parseContent(char* text);
    PARSE_STATE readyWrite();

    bool parseWrite(PARSE_STATE ret);
    bool addResponse(const char* format, ...);
    bool addStatusLine(int status, const char* title);
    bool addHeader(long unsigned int contentLen);
    bool addContentType(const char* type);
    bool addContentLength(long unsigned int contentLen);
    bool addLinger();
    bool addBlankLine();
    bool addContent(const char* content);
    //read
    char readBuf[READ_BUFFER_SIZE];
    long unsigned int readIdx;
    //parseRead
    UPPER_STATE checkState;
    long unsigned int startLine;
    char* text;
    //parseLine
    long unsigned int checkIdx;
    //parseRequestLine
    METHOD method;
    char* url;
    char* version;
    //parseHeader
    long unsigned int contentLength;
    bool linger;
    char* host;
    //parseContent
    char* kvStr;
    //readyWrite
    char realFile[FILE_NAME_LEN];
    char* fileAddr;
    struct stat fileStat;
    //parseRead
    long long bytesHaveSend;
    long long bytesToSend;
    struct iovec iov[2];
    int iovCount;
    //write
    char writeBuf[WRITE_BUFFER_SIZE];
    long unsigned int writeIdx;
};
#endif

