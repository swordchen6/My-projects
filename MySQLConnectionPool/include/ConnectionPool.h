#pragma once
#include<queue>
#include<pthread.h>
#include"cJSON.h"
#include"MysqlConn.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<string.h>
#include<memory>
#include<iostream>
class ConnectionPool
{
public:
	static ConnectionPool* getInstance();//����ģʽ
	ConnectionPool(const ConnectionPool& obj)=delete;
	ConnectionPool& operator=(const ConnectionPool& obj)=delete;
	std::shared_ptr<MysqlConn> getConn();//������
	void receiveConn(MysqlConn* conn);	 //�Ż��߳�
	~ConnectionPool();
	void printMess();
private:
	ConnectionPool();					//���캯��
	bool parseJsonFile();				//����json�ļ�
	void addConn();						//�������,�̷߳ǰ�ȫ
	static void* produceConn(void* arg);//���������߳�
	static void* recycleConn(void* arg);//���������߳�
	std::queue<MysqlConn*> connQueue;	//���Ӷ���
	char* host;							//���ݿ������ip��ַ
	char* user;							//���ݿ��û���
	char* passwd;						//�û�������
	char* dbname;						//���ݿ�����
	unsigned int port;					//�������ݿ�������Ķ˿�
	
	pthread_t producer;					//�������ӵ��̺߳�
	pthread_t reducer;					//�������ӵ��̺߳�

	cJSON* root;						//json�ļ��ĸ��ڵ�

	int currSize;						//�����ȥ������+���е����Ӽ���������
	int minSize;						//��С������
	int	maxSize;						//���������
	int timeOut;						//��ʱʱ��,���뵥λ
	int idleTime;						//������ʱ�������뵥λ
	bool shutDown;						//�Ƿ�ر�

	pthread_mutex_t queueMutex;			//���Ӷ��еĻ�����
	pthread_cond_t produceCond;			//��������������������ʾ��������
	pthread_cond_t reduceCond;			//��������������������ʾ��������
};

