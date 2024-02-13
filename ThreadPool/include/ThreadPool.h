#ifndef _THREADPOOL_H
#define _THREADPOOL_H
#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
//任务结构体
typedef struct Task
{
	void (*function)(void* arg);	//回调函数
	void* arg;						//回调函数参数
}Task;
//线程池结构体
typedef struct ThreadPool
{
	//任务队列
	Task* taskQueue;
	int queueCapacity;			//容量
	int queueSize;				//当前任务个数
	int head;					//队头
	int rear;					//队尾
	//线程池
	pthread_t managerId;		//管理者线程id
	pthread_t* workerIds;		//工作线程组
	int minNum;					//工作线程最小数量
	int maxNum;					//工作线程最大数量
	int busyNum;				//忙状态的工作线程数
	int liveNum;				//目前存活的工作线程数
	int exitNum;				//要杀死的工作线程个数
	//锁
	pthread_mutex_t poolLock;	//线程池锁
	pthread_mutex_t queueLock;	//任务队列锁
	pthread_mutex_t busyLock;	//忙线程数改变频率快，因此单独使用一个锁
	pthread_cond_t notFull;		//消费者通知
	pthread_cond_t notEmpty;	//生产者通知
	//程池状态
	int shutdown;				//销毁为1，不销毁为0
}ThreadPool;
//函数声明
//创建线程池并初始化
ThreadPool* threadPoolCreate(int min,int max,int qc);
//出队操作
Task* pop(ThreadPool* pool);
//入队操作
int push(ThreadPool* pool, Task* obj);
//工作者线程
void* worker(void* arg);
//管理者线程
void* manager(void* arg);
//释放内存
void threadPoolClear(ThreadPool* pool);
//销毁线程池
void threadPoolDestroy(ThreadPool* pool);
//线程退出
void threadExit(ThreadPool* pool);
//给线程池添加任务
void addTask(ThreadPool* pool, Task* obj);
//获取线程池中存活的线程的个数
int getLive(ThreadPool* pool);
//获取线程池中忙状态的线程个数
int getBusy(ThreadPool* pool);
#endif
