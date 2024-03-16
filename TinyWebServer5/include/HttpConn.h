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
//����˼·
//reset�����ı�Ҫ�ԣ���Ϊһ��ʼ��new����HttpConn�����飬��ô���õ����ͷ�һ��Ԫ�ؿռ�
//��ʹ����������Ҳ�˷���ʱ�䡣
//
//read����
    //����ú���ֻ��stateΪWAIT_READ�ſ�ִ��
    //WAIT_READ��ʾδ��������δ��������
    //�Ƴ��ú���������߶�һ�뷵��true����ʧ�ܷ���false
    //��ʧ�ܷ���false���̳߳ص��̻߳���Ϊ���ԶϿ������ӣ�Ȼ��Ӷ�ʱ�����޳�
    //������߶�һ�������һ����process����
//process���� 
    //parseRead������������״̬��
    //  ��״̬�������޳��������ı��ģ���read������һ����������ʱparseLine��������LINE_OPEN
    //      Ȼ��������״̬����ѭ���������˳���״̬����parseRead��������NO_REQUEST
    //      Ȼ��ֱ�Ӵ�process�����˳�����ʱstate��ΪWAIT_READ����ʱҪ�����������¼�
    //  ��״̬���ӽ��յı��Ľ������ݣ���NO_REQUEST,����������ɵ�parseWrite����
    //parseWrite������parseRead�����ķ���ֵ����Ӧ�Ĵ������д�����㹻д������ݷ���true
    //  ���򷵻�false������true,stateΪWAIT_WRITE����ʱӦ����д�¼�
    //  ����false��stateΪPROCESS_ERROR����ʱ�Ͽ����ӣ�Ȼ��Ӷ�ʱ�����޳�
//write����
    //ֻ��stateΪWAIT_WRITE�ſɽ���
    //���ֻ��дһ�룬����true����������д�¼�
    //���дʧ�ܣ�����false�����ԶϿ������ӣ�Ȼ��Ӷ�ʱ�����޳�
    //���д�꣬lingerΪtrue���������ӣ�����true
    //lingerΪfalse���Ͽ����ӣ�Ȼ��Ӷ�ʱ�����޳�������false��

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
    bool read();//�ɹ�������һ����ʧ��ɾ����ʱ�����ر�����
    bool write();
    PROCESS_RET process();
    void reset();

    int fd;                             //��WebServer��dealwithclient��ֵ
    static char root[ROOT_PATH_LEN];    //��WebServer��httpstart��ֵ
    PROCESS_RET state;
    bool improv;                        //�Ƿ񱻴����
    bool cancelTimer;                   //�Ƿ���Ҫɾ����ʱ��
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

