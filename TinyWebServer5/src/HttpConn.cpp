#include "HttpConn.h"
char HttpConn::root[ROOT_PATH_LEN];
//定义http响应的一些状态信息
const char* ok_200_title = "OK";
const char* error_400_title = "Bad Request";
const char* error_400_form = "Your request has bad syntax or is inherently impossible to staisfy.\n";
const char* error_403_title = "Forbidden";
const char* error_403_form = "You do not have permission to get file form this server.\n";
const char* error_404_title = "Not Found";
const char* error_404_form = "The requested file was not found on this server.\n";
const char* error_500_title = "Internal Error";
const char* error_500_form = "There was an unusual problem serving the request file.\n";
HttpConn::HttpConn()
{
	improv = false;
	cancelTimer = false;
	state = WAIT_READ;
	reset();
}
void HttpConn::reset()
{
	bzero(readBuf, READ_BUFFER_SIZE);
	readIdx = 0;

	checkState = CHECK_STATE_REQUESTLINE;
	startLine = 0;
	text = 0;

	checkIdx = 0;

	method = GET;
	url = 0;
	version = 0;

	contentLength = 0;
	linger = false;
	host = 0;

	kvStr = 0;

	bzero(realFile, FILE_NAME_LEN);
	fileAddr = 0;

	bytesHaveSend = 0;
	bytesToSend = 0;
	iovCount = 0;
	iov[0].iov_base = 0;
	iov[1].iov_base = 0;
	iov[0].iov_len = 0;
	iov[1].iov_len = 0;

	bzero(writeBuf, WRITE_BUFFER_SIZE);
	writeIdx = 0;
}

bool HttpConn::read()
{
	LOG_INFO("--HttpConn: Client fd %d read() start\n", fd);
	while (true)
	{
		int bytesRead = recv(fd, readBuf + readIdx, READ_BUFFER_SIZE - readIdx, 0);
		if (bytesRead < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				return true;
			}
			else
			{
				reset();
				return false;
			}
		}
		else if (bytesRead == 0)
		{
			reset();
			return false;
		}
		else
		{
			readIdx += bytesRead;
		}
	}
	return true;
}
#if 0
bool HttpConn::write()
{
	int bytesWrite = 0;
	int newIndex = 0;
	while (1)
	{
		bytesWrite = writev(fd, iov, iovCount);
		if (bytesWrite >= 0)
		{
			bytesHaveSend += bytesWrite;
			newIndex = bytesHaveSend - writeIdx;
		}
		else
		{
			if (errno == EAGAIN)
			{
				if (bytesHaveSend >= iov[0].iov_len)
				{
					iov[0].iov_len = 0;
					iov[1].iov_base = fileAddr + newIndex;
					iov[1].iov_len = bytesToSend;
				}
				else
				{
					iov[0].iov_base = writeBuf + bytesHaveSend;
					iov[0].iov_len = iov[0].iov_len - bytesHaveSend;
				}
				return true;
			}
			if (fileAddr)
			{
				munmap(fileAddr, fileStat.st_size);
			}
			state = WAIT_READ;
			reset();
			return false;
		}
		bytesToSend -= bytesWrite;
		if (bytesToSend <= 0)
		{
			if (fileAddr)
			{
				munmap(fileAddr, fileStat.st_size);
			}
			state = WAIT_READ;
			if (linger)
			{
				reset();
				return true;
			}
			return false;
		}
	}
	return true;
}
#endif

#if 1
bool HttpConn::write()
{
	//写被中断，state仍为WAIT_WRITE，返回true
	//写出错，state应为WAIT_READ,返回false
	//写完，linger为false，state应为WAIT_READ,返回false
	//写完，linger为true，state应为WAIT_READ,返回true
	LOG_INFO("--HttpConn: Client fd %d write() start\n", fd);
	int bytesWrite = 0;
	LOG_INFO("--HttpConn: Client fd %d: bytesHaveSend:%d,\
		bytesToSend:%d\n", fd,bytesHaveSend,bytesToSend);
	while (bytesToSend>0)
	{
		bytesWrite = writev(fd, iov, iovCount);
		if (bytesWrite < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				if (bytesHaveSend >= iov[0].iov_len)
				{
					iov[0].iov_len = 0;
					iov[1].iov_base = fileAddr + bytesHaveSend - writeIdx;
					iov[1].iov_len = bytesToSend;
				}
				else
				{
					iov[0].iov_len = writeIdx - bytesHaveSend;
					iov[0].iov_base = writeBuf + bytesHaveSend;
				}
				break;
			}
			if (fileAddr)
			{
				munmap(fileAddr, fileStat.st_size);
			}
			reset();
			return false;
		}
		else if (bytesWrite >=0)
		{
			bytesHaveSend += bytesWrite;
			bytesToSend -= bytesWrite;
		}
	}
	if (bytesToSend<=0)
	{
		if (fileAddr)
		{
			munmap(fileAddr, fileStat.st_size);
		}
		if (linger)
		{
			state = WAIT_READ;
			reset();
			return true;
		}
		reset();
		return false;
	}
	return true;
}
#endif
HttpConn::PROCESS_RET HttpConn::process()
{
	LOG_INFO("--HttpConn: Client fd %d process() start\n", fd);
	PARSE_STATE readRet = parseRead();
	//readRet可能的值，NO_REQUEST，BAD_REQUEST，FILE_REQUEST,NO_RESOURCE,FORBIDDEN_REQUEST
	if (readRet == NO_REQUEST)
	{
		return WAIT_READ;
	}
	bool writeRet = parseWrite(readRet);
	if (writeRet == false)
	{
		reset();
		return PROCESS_ERROR;
	}
	return WAIT_WRITE;
}

HttpConn::BOTTOM_STATE HttpConn::parseLine()
{
	while (checkIdx < readIdx)
	{
		char temp = readBuf[checkIdx];
		if (temp == '\r')
		{
			if (checkIdx + 1 == readIdx)
			{
				return LINE_OPEN;
			}
			else if(readBuf[checkIdx+1]=='\n')
			{
				readBuf[checkIdx++] = '\0';
				readBuf[checkIdx++] = '\0';
				return LINE_OK;
			}
			return LINE_BAD;
		}
		else if (temp == '\n')
		{
			if (checkIdx > 1 && readBuf[checkIdx - 1] == '\r')
			{
				readBuf[checkIdx - 1] = '\0';
				readBuf[checkIdx++] = '\0';
				return LINE_OK;
			}
			return LINE_BAD;
		}
		checkIdx++;
	}
	return LINE_OPEN;
}

HttpConn::PARSE_STATE HttpConn::parseRead()
{
	LOG_INFO("--HttpConn: Client fd %d parseRead() start\n", fd);
	BOTTOM_STATE lineStatus = LINE_OK;
	PARSE_STATE ret = NO_REQUEST;
	char* text = 0;
	while ((checkState == CHECK_STATE_CONTENT && lineStatus == LINE_OK)
		|| (lineStatus = parseLine()) == LINE_OK)
	{
		text = readBuf + startLine;
		startLine = checkIdx;
		switch (checkState)
		{
		case HttpConn::CHECK_STATE_REQUESTLINE:
			ret = parseRequestLine(text);
			if (ret == BAD_REQUEST)
			{
				return ret;
			}
			break;
		case HttpConn::CHECK_STATE_HEADER:
			ret = parseHeader(text);
			if (ret == BAD_REQUEST)
				return ret;
			else if (ret == GET_REQUEST)
			{
				return readyWrite();
			}
			break;
		case HttpConn::CHECK_STATE_CONTENT:
			ret = parseContent(text);
			if (ret == GET_REQUEST)
			{
				return readyWrite();
			}
			lineStatus = LINE_OPEN;
			break;
		default:
			return INTERNAL_ERROR;
		}
	}
	return NO_REQUEST;
}

HttpConn::PARSE_STATE HttpConn::parseRequestLine(char* text)
{
	LOG_INFO("--HttpConn: Client fd %d requestLine:%s \n", fd,text);
	url = strpbrk(text, " \t");
	if (url == NULL)
	{
		return BAD_REQUEST;
	}
	*url++ = '\0';
	char* methodStr = text;
	url += strspn(url, " \t");
	version = strpbrk(url, " \t");
	if (version == NULL)
	{
		return BAD_REQUEST;
	}
	*version++ = '\0';
	version += strspn(version, " \t");

	if (strcasecmp(methodStr, "GET") == 0)
	{
		method = GET;
	}
	else if (strcasecmp(methodStr, "POST") == 0)
	{
		method = POST;
	}
	else
	{
		return BAD_REQUEST;
	}

	//对请求资源前7个字符进行判断
	//这里主要是有些报文的请求资源中会带有http://，这里需要对这种情况进行单独处理
	if (strncasecmp(url, "http://", 7) == 0)
	{
		url += 7;
		url = strchr(url, '/');
	}
	if (strncasecmp(url, "https://",8) == 0)
	{
		url += 8;
		url = strchr(url, '/');
	}
	//一般的不会带有上述两种符号，直接是单独的/或/后面带访问资源
	if (url == NULL || url[0] != '/')
	{
		return BAD_REQUEST;
	}
	if (strcasecmp(version, "HTTP/1.1") != 0)
	{
		return BAD_REQUEST;
	}
	if (strlen(url) == 1)
	{
		strcat(url, "judge.html");
	}
	checkState = CHECK_STATE_HEADER;
	return NO_REQUEST;
	/* /            GET请求，跳转到judge.html，即欢迎访问页面
	*  /0           POST请求，跳转到register.html，即注册页面
	*  /1           POST请求，跳转到log.html，即登录页面 
	*  /2CGISQL.cgi POST请求，进行登录校验,验证成功跳转到welcome.html，验证失败跳转到logError.html
	*  /3CGISQL.cgi POST请求，进行注册校验,注册成功跳转到log.html，注册失败跳转到registerError.html
	*  /5           POST请求，跳转到picture.html，即图片请求页面
	*  /6           POST请求，跳转到video.html，即视频请求页面
	*  /7           POST请求，跳转到fans.html，即关注页面
	*/
}


HttpConn::PARSE_STATE HttpConn::parseHeader(char* text)
{
	LOG_INFO("--HttpConn: Client fd %d header:%s \n", fd,text);
	if (text[0] == '\0')
	{
		if (contentLength != 0)
		{
			checkState = CHECK_STATE_CONTENT;
			return NO_REQUEST;
		}
		return GET_REQUEST;
	}
	else if (strncasecmp(text, "Connection:", 11) == 0)
	{
		text += 11;
		text += strspn(text, " \t");
		if (strcasecmp(text, "keep-alive") == 0)
		{
			linger = true;
		}
	}
	else if (strncasecmp(text, "Content-length:", 15) == 0)
	{
		text += 15;
		text += strspn(text, " \t");
		contentLength = atoi(text);
	}
	else if (strncasecmp(text, "Host:", 5) == 0)
	{
		text += 5;
		text += strspn(text, " \t");
		host = text;
	}
	return NO_REQUEST;
}

HttpConn::PARSE_STATE HttpConn::parseContent(char* text)
{
	LOG_INFO("--HttpConn: Client fd %d content:%s \n", fd,text);
	if (readIdx >= (checkIdx + contentLength))
	{
		text[contentLength] = '\0';
		kvStr = text;
		return GET_REQUEST;
	}
	return NO_REQUEST;
}

HttpConn::PARSE_STATE HttpConn::readyWrite()
{
	LOG_INFO("--HttpConn: Client fd %d readyWrite() start\n", fd);
	strcpy(realFile, root);
	size_t len = strlen(root);
	const char* p = strrchr(url, '/');

	if (*(p + 1) == '2' || *(p + 1) == '3')
	{
		char name[32];
		char passwd[64];
		//将用户名和密码提取出来
		//user=chen&password=123456
		int i = 0;
		for (i = 5; kvStr[i] != '&'; i++)
		{
			name[i - 5] = kvStr[i];
		}
		name[i - 5] = '\0';
		printf("name: %s\n", name);
		int j = 0;
		for (j = 0, i = i + 10; kvStr[i] != '\0'; i++, j++)
		{
			passwd[j] = kvStr[i];
		}
		passwd[j] = '\0';
		printf("passwd: %s\n", passwd);
		//如果是登录，直接判断
		//若浏览器端输入的用户名和密码在表中可以查找到，返回1，否则返回0
		char sql[128] = { 0 };
		std::shared_ptr<MysqlConn> conn = ConnPool::singleton()->getConn();
		if (*(p + 1) == '2')
		{
			snprintf(sql, 128, "select * from user where username= '%s' and passwd='%s'",
				name, passwd);
			mysql_query(conn->get(), sql);
			MYSQL_RES* result = mysql_store_result(conn->get());
			long unsigned int rows = mysql_num_rows(result);
			if (rows == 1)
			{
				strcpy(url, "/welcome.html");
			}
			else
			{
				strcpy(url, "/logError.html");
			}
			mysql_free_result(result);
		}

		if (*(p + 1) == '3')
		{
			snprintf(sql, 128, "select * from user where username= '%s' and passwd='%s'",
				name, passwd);
			mysql_query(conn->get(), sql);
			MYSQL_RES* result = mysql_store_result(conn->get());
			long unsigned int rows = mysql_num_rows(result);
			if (rows == 1)
			{
				strcpy(url, "/registerError.html");
			}
			else
			{
				snprintf(sql, 128, "insert into user values('%s','%s')",
					name, passwd);
				mysql_query(conn->get(), sql);
				strcpy(url, "/log.html");
			}
			mysql_free_result(result);
		}
	}

	switch (*(p + 1))
	{
	case '0':
		strcpy(realFile + len, "/register.html");
		break;
	case '1':
		strcpy(realFile + len, "/log.html");
		break;
	case '5':
		strcpy(realFile + len, "/picture.html");
		break;
	case '6':
		strcpy(realFile + len, "/video.html");
		break;
	case '7':
		strcpy(realFile + len, "/fans.html");
		break;
	default:
		strcpy(realFile + len, url);
	}

	if (stat(realFile, &fileStat) < 0)
	{
		return NO_RESOURCE;
	}
	if (!(fileStat.st_mode & S_IROTH))
	{
		return FORBIDDEN_REQUEST;
	}
	if (S_ISDIR(fileStat.st_mode))
	{
		return BAD_REQUEST;
	}

	int fileFd = open(realFile, O_RDONLY);
	fileAddr = (char*)mmap(0, fileStat.st_size, PROT_READ, MAP_PRIVATE, fileFd, 0);
	close(fileFd);

	return FILE_REQUEST;
}

bool HttpConn::parseWrite(PARSE_STATE ret)
{
	//readRet可能的值，NO_REQUEST，BAD_REQUEST，FILE_REQUEST,
	//NO_RESOURCE,FORBIDDEN_REQUEST
	LOG_INFO("--HttpConn: Client fd %d parseWrite() start\n", fd);
	switch (ret)
	{
	case BAD_REQUEST:
		addStatusLine(404, error_404_title);
		addHeader(strlen(error_404_form));
		if (!addContent(error_404_form))
			return false;
		break;
	case INTERNAL_ERROR:
		addStatusLine(500, error_500_title);
		addHeader(strlen(error_500_form));
		if (!addContent(error_500_form))
			return false;
		break;
	case NO_RESOURCE:
		addStatusLine(404, error_404_title);
		addHeader(strlen(error_404_form));
		if (!addContent(error_404_form))
			return false;
		break;
	case FORBIDDEN_REQUEST:
		addStatusLine(403, error_403_title);
		addHeader(strlen(error_403_form));
		if (!addContent(error_403_form))
			return false;
		break;
	case FILE_REQUEST:
		addStatusLine(200, ok_200_title);
		if (fileStat.st_size != 0)
		{
			addHeader(fileStat.st_size);
			for (int i = 0; i < writeIdx; i++)
			{
				if (writeBuf[i] == '\r')
				{
					printf("\\r");
				}
				else if (writeBuf[i] == '\n')
				{
					printf("\\n\n");
				}
				else
				{
					printf("%c", writeBuf[i]);
				}
			}
			iov[0].iov_base = writeBuf;
			iov[0].iov_len = writeIdx;
			iov[1].iov_base = fileAddr;
			iov[1].iov_len = fileStat.st_size;
			iovCount = 2;
			bytesToSend = writeIdx + fileStat.st_size;
			return true;
		}
		else
		{
			const char* ok_string = "<html><body></body></html>";
			addHeader(strlen(ok_string));
			if (!addContent(ok_string))
				return false;
		}
	default:
		return false;
	}
	iov[0].iov_base = writeBuf;
	iov[0].iov_len = writeIdx;
	iovCount = 1;
	bytesToSend = writeIdx;
	return true;
}

bool HttpConn::addResponse(const char* format, ...)
{
	if (writeIdx >= WRITE_BUFFER_SIZE)
	{
		return false;
	}

	va_list valist;
	va_start(valist, format);
	long unsigned int len = vsnprintf(writeBuf + writeIdx, 
		WRITE_BUFFER_SIZE - writeIdx, format, valist);

	writeIdx += len;
	va_end(valist);
	return true;
}

bool HttpConn::addStatusLine(int status, const char* title)
{
	return addResponse("%s %d %s\r\n", "HTTP/1.1", status, title);
}

bool HttpConn::addHeader(long unsigned int contentLen)
{
	return addContentLength(contentLen) && addLinger() && addBlankLine();
}

bool HttpConn::addContentType(const char* type)
{
	return addResponse("Content-Type:%s\r\n", type);
}

bool HttpConn::addContentLength(long unsigned int contentLen)
{
	return addResponse("Content-Length:%d\r\n", contentLen);
}

bool HttpConn::addLinger()
{
	return addResponse("Connection:%s\r\n", linger == true ? "keep-alive" : "close"); 
}

bool HttpConn::addBlankLine()
{
	return addResponse("\r\n");
}

bool HttpConn::addContent(const char* content)
{
	return addResponse("%s", content);
}
