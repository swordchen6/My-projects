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
//大致思路：
//将其分成两部分，一部分为读和解析，另一部分为写
//1.读和解析
//情况1：read函数失败，在HttpConn将调用reset函数变为原始状态，在这里标记要断开连接，删除定时器
//情况2：read函数成功，但是只读了一半，state仍为WAIT_READ，到webserver判断继续监听读事件
//情况3：read函数成功，读完全部数据，但是写缓冲不够，state为PROCESS_ERROR，在这里标记断连删定
//情况4：read函数成功，读完全部数据，并且写缓冲足够，state为WAIT_WRITE，到webserver判断监听写事件
//2.写
//写是在情况4的情况下，监听到写事件，继续执行
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
