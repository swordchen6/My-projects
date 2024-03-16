#ifndef _TIMER_H_
#define _TIMER_H_
#include<arpa/inet.h>
#include<exception>
#include<algorithm>
#include<time.h>
#include<stdio.h>
#include<vector>
//可以根据fd找到UserData，
//通过UserData找到Timer，可以成员index和TimerHeap对外的接口对fd对应的Timer进行操作
struct UserData
{
	int index;
	int fd;
	sockaddr_in addr;
};

struct Timer
{
	long long expire;
	UserData* data;
};

class TimerHeap
{
public:
	const static int TIMESLOT = 5;
	TimerHeap(int pMaxSize);
	~TimerHeap();
	bool setTimer(int fd, sockaddr_in addr,UserData* ud);
	int delTimer(int index);
	bool adjustTimer(int index);
	std::vector<int> tick();
private:
	void upwardAdjust(int index);
	void downwardAjust(int index);

	Timer* heap;
	int maxSize;
	int curSize;
};

#endif

