#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_
#include<pthread.h>
#include<list>
#include<exception>
#include"Locker.h"
#include"HttpConn.h"
template<class T>
class ThreadPool
{
public:
	void init(int pThreadNum, long unsigned int pMaxRequest);
	static ThreadPool<T>* singleton();
	~ThreadPool();
	bool append(T* request);
private:
	ThreadPool() = default;
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	static void* worker(void* arg);

	pthread_t* threads;
	int threadNum;

	std::list<T*> requestQueue;
	int maxRequest;

	Mutex mutex;
	Sem queueStat;

	bool shutDown;
};

template<class T>
void ThreadPool<T>::init(int pThreadNum, long unsigned int pMaxRequest)
{
	if (pThreadNum <= 0 || pMaxRequest <= 0)
	{
		throw std::exception();
	}
	threadNum = pThreadNum;
	maxRequest = pMaxRequest;
	shutDown = false;

	threads = new pthread_t[threadNum];
	if (threads == NULL)
	{
		throw std::exception();
	}
	for (int i = 0; i < threadNum; i++)
	{
		if (pthread_create(threads + i, NULL, worker, this) != 0)
		{
			delete[]threads;
			throw std::exception();
		}
		if (pthread_detach(threads[i])!=0)
		{
			delete[]threads;
			throw std::exception();
		}
	}
}

template<class T>
ThreadPool<T>* ThreadPool<T>::singleton()
{
	static ThreadPool pool;
	return &pool;
}

template<class T>
ThreadPool<T>::~ThreadPool()
{
	mutex.lock();
	shutDown = true;
	if (threads)
	{
		delete[]threads;
	}
	for (int i = 0; i < threadNum; i++)
	{
		queueStat.post();
	}
	mutex.unlock();
}

template<class T>
bool ThreadPool<T>::append(T* request)
{
	if (!request)
	{
		return false;
	}
	mutex.lock();
	if (requestQueue.size() >= maxRequest)
	{
		mutex.unlock();
		return false;
	}
	requestQueue.push_back(request);
	mutex.unlock();
	queueStat.post();
	return true;
}
//����˼·��
//����ֳ������֣�һ����Ϊ���ͽ�������һ����Ϊд
//1.���ͽ���
//���1��read����ʧ�ܣ���HttpConn������reset������Ϊԭʼ״̬����������Ҫ�Ͽ����ӣ�ɾ����ʱ��
//���2��read�����ɹ�������ֻ����һ�룬state��ΪWAIT_READ����webserver�жϼ����������¼�
//���3��read�����ɹ�������ȫ�����ݣ�����д���岻����stateΪPROCESS_ERROR���������Ƕ���ɾ��
//���4��read�����ɹ�������ȫ�����ݣ�����д�����㹻��stateΪWAIT_WRITE����webserver�жϼ���д�¼�
//2.д
//д�������4������£�������д�¼�������ִ��
//
template<class T>
void* ThreadPool<T>::worker(void* arg)
{
	ThreadPool* pool = (ThreadPool*)arg;
	while (!pool->shutDown)
	{
		pool->queueStat.wait();
		pool->mutex.lock();
		if (pool->shutDown)
		{
			pool->mutex.unlock();
			continue;
		}
		T* request = pool->requestQueue.front();
		pool->requestQueue.pop_front();
		pool->mutex.unlock();
		LOG_INFO("--ThreadPool: Client fd % d worker() start\n", request->fd);
		if (request->state == HttpConn::WAIT_READ)
		{
			LOG_INFO("--ThreadPool: Client fd % d will read\n", request->fd);
			if (request->read())
			{
				LOG_INFO("--ThreadPool: Client fd % d will process\n", request->fd);
				request->state = request->process();
				if (request->state == HttpConn::PROCESS_ERROR)
				{
					request->cancelTimer = true;
					request->state = HttpConn::WAIT_READ;
				}
			}
			else
			{
				request->cancelTimer = true;
			}
		}
		else if(request->state == HttpConn::WAIT_WRITE)
		{
			LOG_INFO("--ThreadPool: Client fd % d will write\n", request->fd);
			if (request->write() == false)
			{
				request->state = HttpConn::WAIT_READ;
				request->cancelTimer = true;
			}
		}
		request->improv = true;
	}
	return NULL;
}

#endif
