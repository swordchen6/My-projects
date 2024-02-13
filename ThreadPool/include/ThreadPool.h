#ifndef _THREADPOOL_H
#define _THREADPOOL_H
#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
//����ṹ��
typedef struct Task
{
	void (*function)(void* arg);	//�ص�����
	void* arg;						//�ص���������
}Task;
//�̳߳ؽṹ��
typedef struct ThreadPool
{
	//�������
	Task* taskQueue;
	int queueCapacity;			//����
	int queueSize;				//��ǰ�������
	int head;					//��ͷ
	int rear;					//��β
	//�̳߳�
	pthread_t managerId;		//�������߳�id
	pthread_t* workerIds;		//�����߳���
	int minNum;					//�����߳���С����
	int maxNum;					//�����߳��������
	int busyNum;				//æ״̬�Ĺ����߳���
	int liveNum;				//Ŀǰ���Ĺ����߳���
	int exitNum;				//Ҫɱ���Ĺ����̸߳���
	//��
	pthread_mutex_t poolLock;	//�̳߳���
	pthread_mutex_t queueLock;	//���������
	pthread_mutex_t busyLock;	//æ�߳����ı�Ƶ�ʿ죬��˵���ʹ��һ����
	pthread_cond_t notFull;		//������֪ͨ
	pthread_cond_t notEmpty;	//������֪ͨ
	//�̳�״̬
	int shutdown;				//����Ϊ1��������Ϊ0
}ThreadPool;
//��������
//�����̳߳ز���ʼ��
ThreadPool* threadPoolCreate(int min,int max,int qc);
//���Ӳ���
Task* pop(ThreadPool* pool);
//��Ӳ���
int push(ThreadPool* pool, Task* obj);
//�������߳�
void* worker(void* arg);
//�������߳�
void* manager(void* arg);
//�ͷ��ڴ�
void threadPoolClear(ThreadPool* pool);
//�����̳߳�
void threadPoolDestroy(ThreadPool* pool);
//�߳��˳�
void threadExit(ThreadPool* pool);
//���̳߳��������
void addTask(ThreadPool* pool, Task* obj);
//��ȡ�̳߳��д����̵߳ĸ���
int getLive(ThreadPool* pool);
//��ȡ�̳߳���æ״̬���̸߳���
int getBusy(ThreadPool* pool);
#endif
