#include "Log.h"

Log::~Log()
{
	mutex.lock();
	shutDown = true;
	if (lineQueue)
	{
		delete lineQueue;
	}
	if (fp)
	{
		fclose(fp);
	}
	if (buffer)
	{
		delete[]buffer;
	}
	mutex.unlock();
}

Log* Log::singleton()
{
	static Log l;
	return &l;
}

void Log::init(const char* file, long long pMaxLine, int pMaxQueueSize)
{
	shutDown = false;
	maxLine = pMaxLine;
	curLine = 0;
	buffer = new char[BUFFER_SIZE];
	if (!buffer)
	{
		throw std::exception();
	}
	memset(dirName, 0, DIR_NAME_LEN);
	memset(logName, 0, LOG_NAME_LEN);
	memset(buffer, 0, BUFFER_SIZE);
	time_t diff = time(NULL);
	struct tm point = *(localtime(&diff));
	curDay = point.tm_mday;

	char timeStr[16] = { 0 };
	snprintf(timeStr, 16, "%d_%02d_%02d", point.tm_year+1900, 
		point.tm_mon+1, point.tm_mday);

	const char* p = strrchr(file, '/');
	if (p == NULL)
	{
		if (!getcwd(dirName, DIR_NAME_LEN))
		{
			throw std::exception();
		}
		strcat(dirName, "/");
		strcpy(logName, file);
	}
	else
	{
		strncpy(dirName, file, p + 1 - file);
		strcpy(logName, p + 1);
	}
	char fullName[288] = { 0 };
	snprintf(fullName, 288, "%s%s_%s_1.log", dirName, timeStr, logName);
	fp = fopen(fullName, "a");
	if (fp == NULL)
	{
		throw std::exception();
	}

	if (pMaxQueueSize > 0)
	{
		maxQueueSize = pMaxQueueSize;
		lineQueue = new BlockQueue<std::string>(maxQueueSize);
		pthread_t tid;
		if (pthread_create(&tid, NULL, asyncWriteLog, NULL) != 0)
		{
			throw std::exception();
		}
		if (pthread_detach(tid) != 0)
		{
			throw std::exception();
		}
	}
}

bool Log::isShutDown()
{
	bool mark = true;
	mutex.lock();
	mark = shutDown;
	mutex.unlock();
	return mark;
}

void Log::writeLog(int level, const char* format, ...)
{
	char levelStr[16] = { 0 };
	switch (level)
	{
	case 0:
		strcpy(levelStr, "[debug]:"); break;
	case 1:
		strcpy(levelStr, "[info]:"); break;
	case 2:
		strcpy(levelStr, "[warn]:"); break;
	case 3:
		strcpy(levelStr, "[error]:"); break;
	default:
		strcpy(levelStr, "[info]:"); break;
	}

	struct timeval t = { 0,0 };
	gettimeofday(&t, NULL);
	struct tm point = *(localtime(&t.tv_sec));
	char fullName[288] = { 0 };
	char timeStr[16] = { 0 };

	mutex.lock();
	if (point.tm_mday != curDay || (curLine % maxLine == 0 && curLine > 0))
	{
		fflush(fp);
		fclose(fp);
		snprintf(timeStr, 16, "%d_%02d_%02d", point.tm_year+1900, 
			point.tm_mon+1, point.tm_mday);
		if (point.tm_mday != curDay)
		{
			curDay = point.tm_mday;
			curLine = 0;
			snprintf(fullName, 288, "%s%s_%s_1.log", dirName, timeStr, logName);
		}
		else if((curLine % maxLine == 0 && curLine > 0))
		{
			snprintf(fullName, 288, "%s%s_%s_%lld.log", dirName, 
				timeStr, logName, curLine / maxLine + 1);
		}
		fp = fopen(fullName, "a");
		if (!fp)
		{
			throw std::exception();
		}
	}
	mutex.unlock();

	va_list valist;
	va_start(valist, format);
	mutex.lock();
	
	int writeNum1 = snprintf(buffer, BUFFER_SIZE, "%s %d-%02d-%02d %02d:%02d:%02d.%06ld ",
		levelStr, point.tm_year + 1900, point.tm_mon + 1, point.tm_mday, point.tm_hour, 
		point.tm_min,point.tm_sec, t.tv_usec);
	int writeNum2=vsnprintf(buffer+ writeNum1, BUFFER_SIZE- writeNum1-1, format, valist);
	buffer[writeNum1 + writeNum2] = '\n';
	buffer[writeNum1 + writeNum2 + 1] = '\0';

	curLine++;
	mutex.unlock();

	va_end(valist);
	std::string lineStr = buffer;	
	if (maxQueueSize > 0)
	{
		lineQueue->push(lineStr);
	}
	else
	{
		mutex.lock();
		fputs(lineStr.c_str(),fp);
		mutex.unlock();
	}
}

void Log::flush()
{
	mutex.lock();
	fflush(fp);
	mutex.unlock();
}

void* Log::asyncWriteLog(void* arg)
{
	Log* log = Log::singleton();
	std::string oneLine;
	while (!log->shutDown&&log->lineQueue->pop(oneLine))
	{
		if (log->shutDown)
		{
			break;
		}
		log->mutex.lock();
		fputs(oneLine.c_str(), log->fp);
		log->mutex.unlock();
	}
	return NULL;
}
