#include "Locker.h"

Mutex::Mutex()
{
	if (pthread_mutex_init(&mutex, NULL) != 0)
	{
		throw std::exception();
	}
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&mutex);
}

bool Mutex::lock()
{
	return pthread_mutex_lock(&mutex) == 0;
}

bool Mutex::unlock()
{
	return pthread_mutex_unlock(&mutex) == 0;
}

pthread_mutex_t* Mutex::get()
{
	return &mutex;
}

Cond::Cond()
{
	if (pthread_cond_init(&cond, NULL) != 0)
	{
		throw std::exception();
	}
}

Cond::~Cond()
{
	pthread_cond_destroy(&cond);
}

bool Cond::wait(pthread_mutex_t* mutexPtr)
{
	return pthread_cond_wait(&cond, mutexPtr) == 0;
}

bool Cond::signal()
{
	return pthread_cond_signal(&cond) == 0;
}

bool Cond::broadcast()
{
	return pthread_cond_broadcast(&cond) == 0;
}

Sem::Sem(int num)
{
	if (sem_init(&sem, 0, num) != 0)
	{
		throw std::exception();
	}
}

Sem::~Sem()
{
	sem_destroy(&sem);
}

bool Sem::wait()
{
	return sem_wait(&sem) == 0;
}

bool Sem::post()
{
	return sem_post(&sem) == 0;
}
