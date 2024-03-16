#ifndef _BLOCK_QUEUE_H_
#define _BLOCK_QUEUE_H_
#include<exception>
#include<stdio.h>
#include"Locker.h"
template<class T>
class BlockQueue
{
public:
	BlockQueue(int pMaxSize);
	~BlockQueue();
	void clear();
	int getMaxSize();
	int getCurSize();
	bool getFront(T& obj);
	bool push(const T& obj);
	bool pop(T& obj);
private:
	T* bQueue;
	int maxSize;
	int curSize;
	int front;
	int back;
	bool shutDown;
	Mutex mutex;
	Cond pushCond;
	Cond popCond;
};

template<class T>
BlockQueue<T>::BlockQueue(int pMaxSize)
{
	shutDown = false;
	maxSize = pMaxSize;
	curSize = 0;
	front = back = 0;
	bQueue = new T[maxSize];
	if (bQueue == NULL)
	{
		throw std::exception();
	}
}

template<class T>
BlockQueue<T>::~BlockQueue()
{
	mutex.lock();
	shutDown = true;
	if (bQueue)
	{
		delete[] bQueue;
	}
	mutex.unlock();
	pushCond.broadcast();
	popCond.broadcast();
}

template<class T>
inline void BlockQueue<T>::clear()
{
	mutex.lock();
	curSize = 0;
	front = back = 0;
	mutex.unlock();
}

template<class T>
int BlockQueue<T>::getMaxSize()
{
	int ms = 0;
	mutex.lock();
	ms = maxSize;
	mutex.unlock();
	return ms;
}

template<class T>
int BlockQueue<T>::getCurSize()
{
	int cs = 0;
	mutex.lock();
	cs = curSize;
	mutex.unlock();
	return cs;
}

template<class T>
bool BlockQueue<T>::getFront(T& obj)
{
	mutex.lock();
	if (curSize <= 0)
	{
		mutex.unlock();
		return false;
	}
	obj = bQueue[front];
	mutex.unlock();
	return true;
}

template<class T>
bool BlockQueue<T>::push(const T& obj)
{
	mutex.lock();
	while (!shutDown&&curSize >= maxSize)
	{
		if (!pushCond.wait(mutex.get()))
		{
			mutex.unlock();
			return false;
		}
	}
	if (shutDown)
	{
		mutex.unlock();
		return false;
	}
	bQueue[back] = obj;
	back = (back + 1) % maxSize;
	curSize++;
	mutex.unlock();
	popCond.signal();
	return true;
}

template<class T>
bool BlockQueue<T>::pop(T& obj)
{
	mutex.lock();
	while (!shutDown&&curSize <= 0)
	{
		if (!popCond.wait(mutex.get()))
		{
			mutex.unlock();
			return false;
		}
	}
	if (shutDown)
	{
		mutex.unlock();
		return false;
	}
	obj = bQueue[front];
	front = (front + 1) % maxSize;
	curSize--;
	mutex.unlock();
	pushCond.signal();
	return true;
}

#endif
