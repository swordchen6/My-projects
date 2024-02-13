#include "ThreadPool.h"
//管理者线程每次增加线程只增加两个，这个可以根据实际情况修改
const int TIME_ADD = 2;
//管理者线程每次销毁线程只销毁两个，这个可以根据实际情况修改
const int TIME_MINUS = 2;
ThreadPool* threadPoolCreate(int min, int max, int qc)
{
	ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));
	do
	{
		if (pool == NULL)
		{
			printf("malloc threadPool fail...\n");
			break;
		}
		pool->workerIds = (pthread_t*)malloc(sizeof(pthread_t) * max);
		if (pool->workerIds == NULL)
		{
			printf("malloc workIds fail...\n");
			break;
		}
		memset(pool->workerIds, 0, max * sizeof(pthread_t));
		pool->minNum = min;
		pool->maxNum = max;
		pool->busyNum = 0;
		pool->liveNum = min;
		pool->exitNum = 0;

		if (pthread_mutex_init(&pool->poolLock, NULL) != 0 ||
			pthread_mutex_init(&pool->busyLock, NULL) != 0 ||
			pthread_mutex_init(&pool->queueLock, NULL) != 0 ||
			pthread_cond_init(&pool->notEmpty, NULL) != 0 ||
			pthread_cond_init(&pool->notFull, NULL) != 0)
		{
			printf("mutex or cond init fail...\n");
			break;
		}

		//任务队列
		pool->taskQueue = (Task*)malloc(sizeof(Task) * qc);
		if (pool->taskQueue == NULL)
		{
			printf("malloc taskQueue fail....\n");
			break;
		}
		pool->queueCapacity = qc;
		pool->queueSize = 0;
		pool->head = 0;
		pool->rear = 0;
		pool->shutdown = 0;

		//创建线程
		pthread_create(&pool->managerId, NULL, manager, pool);
		for (int i = 0; i < min; i++)
		{
			pthread_create(&pool->workerIds[i], NULL, worker, pool);
		}
		return pool;
	} while (0);

	threadPoolClear(pool);
	return NULL;
}

Task* pop(ThreadPool* pool)
{
	pthread_mutex_lock(&pool->queueLock);
	if (pool->queueSize <= 0)
	{
		pthread_mutex_unlock(&pool->queueLock);
		return NULL;
	}
	Task* obj = &pool->taskQueue[pool->head];
	pool->head = (pool->head + 1) % pool->queueCapacity;
	pool->queueSize--;
	pthread_mutex_unlock(&pool->queueLock);
	return obj;
}

int push(ThreadPool* pool, Task* obj)
{
	pthread_mutex_lock(&pool->queueLock);
	if (pool->queueCapacity <= pool->queueSize)
	{
		pthread_mutex_unlock(&pool->queueLock);
		return 0;
	}
	pool->taskQueue[pool->rear].function = obj->function;
	pool->taskQueue[pool->rear].arg = obj->arg;
	pool->rear = (pool->rear + 1) % pool->queueCapacity;
	pool->queueSize++;
	pthread_mutex_unlock(&pool->queueLock);
	return 1;
}

void* worker(void* arg)
{
	ThreadPool* pool = (ThreadPool*)arg;
	while (1)
	{
		pthread_mutex_lock(&pool->poolLock);
		//任务队列非空
		while (pool->shutdown == 0 && pool->queueSize == 0)
		{
			//阻塞线程
			pthread_cond_wait(&pool->notEmpty, &pool->poolLock);
			if (pool->exitNum > 0)
			{
				pool->exitNum--;
				if (pool->liveNum > pool->minNum)
				{
					pool->liveNum--;
					pthread_mutex_unlock(&pool->poolLock);
					threadExit(pool);
				}
			}
		}
		//判断线程池是否被关闭了 
		if (pool->shutdown)
		{
			pthread_mutex_unlock(&pool->poolLock);
			//因为线程池关闭要销毁，就不必再到工作id数组将该线程id置为0了
			//直接使用pthread_exit就行
			pthread_exit(NULL);
		}
		//取任务
		Task* obj = pop(pool);
		//解锁
		pthread_mutex_unlock(&pool->poolLock);
		//完成任务
		pthread_mutex_lock(&pool->busyLock);
		pool->busyNum++;
		pthread_mutex_unlock(&pool->busyLock);
		
		obj->function(obj->arg);

		pthread_mutex_lock(&pool->busyLock);
		pool->busyNum--;
		pthread_mutex_unlock(&pool->busyLock);
		pthread_cond_signal(&pool->notFull);
	}
	return NULL;
}

void* manager(void* arg)
{
	ThreadPool* pool = (ThreadPool*)arg;
	while (!pool->shutdown)
	{
		//每隔三秒检测一次
		sleep(3);
		//取出线程池中任务数量和当前线程的数量
		pthread_mutex_lock(&pool->poolLock);
		int queueSize = pool->queueSize;
		int liveNum = pool->liveNum;
		pthread_mutex_unlock(&pool->poolLock);

		//取出忙的线程数
		pthread_mutex_lock(&pool->busyLock);
		int busyNum = pool->busyNum;
		pthread_mutex_unlock(&pool->busyLock);

		//添加线程，下面的算法可以根据实际情况修改,销毁线程同理
		//任务个数>存活线程个数 && 存活的线程数<最大线程数
		//下面的maxNum是不变的，因此不需要锁
		if (queueSize > liveNum && liveNum < pool->maxNum)
		{
			int count = 0;
			pthread_mutex_lock(&pool->poolLock);
			for (int i = 0; i < pool->maxNum && count < TIME_ADD
				&& pool->liveNum < pool->maxNum; i++)
			{
				if (pool->workerIds[i] == 0)
				{
					pthread_create(&pool->workerIds[i], NULL, worker, pool);
					count++;
					pool->liveNum++;
				}
			}
			pthread_mutex_unlock(&pool->poolLock);
		}
		//销毁线程
		//忙的线程*2<存活的线程&&存活的线程>最小的线程数
		if (busyNum * 2 < liveNum && liveNum > pool->minNum)
		{
			pthread_mutex_lock(&pool->poolLock);
			pool->exitNum = TIME_MINUS;
			pthread_mutex_unlock(&pool->poolLock);
			for (int i = 0; i < TIME_MINUS; i++)
			{
				pthread_cond_signal(&pool->notEmpty);
			}
		}
	}
	return NULL;
}

void threadPoolClear(ThreadPool* pool)
{
	if (pool && pool->taskQueue)
	{
		free(pool->taskQueue);
	}
	if (pool && pool->workerIds)
	{
		free(pool->workerIds);
	}
	if (pool)
	{
		free(pool);
	}
	pthread_mutex_destroy(&pool->busyLock);
	pthread_mutex_destroy(&pool->poolLock);
	pthread_mutex_destroy(&pool->queueLock);
	pthread_cond_destroy(&pool->notEmpty);
	pthread_cond_destroy(&pool->notFull);
	pool = NULL;
}

void threadPoolDestroy(ThreadPool* pool)
{
	if (pool == NULL)
	{
		return;
	}
	//关闭线程池
	pool->shutdown = 1;
	//阻塞回收
	pthread_join(pool->managerId, NULL);
	for (int i = 0; i < pool->liveNum; i++)
	{
		pthread_cond_signal(&pool->notEmpty);
	}
	threadPoolClear(pool);
}

void threadExit(ThreadPool* pool)
{
	pthread_t tid = pthread_self();
	for (int i = 0; i < pool->maxNum; i++)
	{
		if (pool->workerIds[i] == tid)
		{
			pool->workerIds[i] = 0;
			printf("thread %ld exit...\n", tid);
			pthread_exit(NULL);
		}
	}
}

void addTask(ThreadPool* pool, Task* obj)
{
	pthread_mutex_lock(&pool->poolLock);
	while (pool->shutdown == 0 && pool->queueCapacity <= pool->queueSize)
	{
		//阻塞生产者进程
		pthread_cond_wait(&pool->notFull, &pool->poolLock);
	}
	if (pool->shutdown)
	{
		pthread_mutex_unlock(&pool->poolLock);
		return;
	}
	//添加任务
	push(pool, obj);
	pthread_cond_signal(&pool->notEmpty);
	pthread_mutex_unlock(&pool->poolLock);
}

int getLive(ThreadPool* pool)
{
	pthread_mutex_lock(&pool->poolLock);
	int liveNum = pool->busyNum;
	pthread_mutex_unlock(&pool->poolLock);
	return liveNum;
}

int getBusy(ThreadPool* pool)
{
	pthread_mutex_lock(&pool->busyLock);
	int busyNum = pool->busyNum;
	pthread_mutex_unlock(&pool->busyLock);
	return busyNum;
}
