#include "ThreadPool.h"
//�������߳�ÿ�������߳�ֻ����������������Ը���ʵ������޸�
const int TIME_ADD = 2;
//�������߳�ÿ�������߳�ֻ����������������Ը���ʵ������޸�
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

		//�������
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

		//�����߳�
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
		//������зǿ�
		while (pool->shutdown == 0 && pool->queueSize == 0)
		{
			//�����߳�
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
		//�ж��̳߳��Ƿ񱻹ر��� 
		if (pool->shutdown)
		{
			pthread_mutex_unlock(&pool->poolLock);
			//��Ϊ�̳߳عر�Ҫ���٣��Ͳ����ٵ�����id���齫���߳�id��Ϊ0��
			//ֱ��ʹ��pthread_exit����
			pthread_exit(NULL);
		}
		//ȡ����
		Task* obj = pop(pool);
		//����
		pthread_mutex_unlock(&pool->poolLock);
		//�������
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
		//ÿ��������һ��
		sleep(3);
		//ȡ���̳߳������������͵�ǰ�̵߳�����
		pthread_mutex_lock(&pool->poolLock);
		int queueSize = pool->queueSize;
		int liveNum = pool->liveNum;
		pthread_mutex_unlock(&pool->poolLock);

		//ȡ��æ���߳���
		pthread_mutex_lock(&pool->busyLock);
		int busyNum = pool->busyNum;
		pthread_mutex_unlock(&pool->busyLock);

		//����̣߳�������㷨���Ը���ʵ������޸�,�����߳�ͬ��
		//�������>����̸߳��� && �����߳���<����߳���
		//�����maxNum�ǲ���ģ���˲���Ҫ��
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
		//�����߳�
		//æ���߳�*2<�����߳�&&�����߳�>��С���߳���
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
	//�ر��̳߳�
	pool->shutdown = 1;
	//��������
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
		//���������߽���
		pthread_cond_wait(&pool->notFull, &pool->poolLock);
	}
	if (pool->shutdown)
	{
		pthread_mutex_unlock(&pool->poolLock);
		return;
	}
	//�������
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
