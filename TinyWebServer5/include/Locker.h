#ifndef _LOCKER_H_
#define _LOCKER_H_
#include<pthread.h>
#include<semaphore.h>
#include<exception>
#include<stdio.h>
class Mutex
{
public:
	Mutex();
	~Mutex();
	bool lock();
	bool unlock();
	pthread_mutex_t* get();
private:
	pthread_mutex_t mutex;
};
class Cond
{
public:
	Cond();
	~Cond();
	bool wait(pthread_mutex_t* mutexPtr);
	bool signal();
	bool broadcast();
private:
	pthread_cond_t cond;
};
class Sem
{
public:
	Sem(int num = 0);
	~Sem();
	bool wait();
	bool post();
private:
	sem_t sem;
};
#endif

