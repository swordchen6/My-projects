#ifndef _LOG_H_
#define _LOG_H_
#include<string>
#include<string.h>
#include<unistd.h>
#include<stdarg.h>
#include<sys/time.h>
#include"BlockQueue.hpp"
class Log
{
public:
	~Log();
	static Log* singleton();
	void init(const char* file, long long pMaxLine,int pMaxQueueSize);
	bool isShutDown();
	void writeLog(int level, const char* format, ...);
	void flush();
private:
	Log() = default;
	Log(const Log&) = delete;
	Log& operator=(const Log&) = delete;
	static void* asyncWriteLog(void* arg);

	const static int DIR_NAME_LEN = 128;
	const static int LOG_NAME_LEN = 128;
	const static int BUFFER_SIZE = 8192;

	bool shutDown;
	BlockQueue<std::string>* lineQueue;
	int maxQueueSize;
	
	FILE* fp;
	long long maxLine;
	long long curLine;
	int curDay;
	char dirName[DIR_NAME_LEN];
	char logName[LOG_NAME_LEN];
	char* buffer;

	Mutex mutex;
};

#define LOG_DEBUG(format,...) if(!Log::singleton()->isShutDown())\
	{Log::singleton()->writeLog(0,format,##__VA_ARGS__);Log::singleton()->flush();}
#define LOG_INFO(format,...) if(!Log::singleton()->isShutDown())\
	{Log::singleton()->writeLog(1,format,##__VA_ARGS__);Log::singleton()->flush();}
#define LOG_WARN(format,...) if(!Log::singleton()->isShutDown())\
	{Log::singleton()->writeLog(2,format,##__VA_ARGS__);Log::singleton()->flush();}
#define LOG_ERROR(format,...) if(!Log::singleton()->isShutDown())\
	{Log::singleton()->writeLog(3,format,##__VA_ARGS__);Log::singleton()->flush();}
#endif
