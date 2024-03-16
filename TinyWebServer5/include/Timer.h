#ifndef _TIMER_H_
#define _TIMER_H_
#include<arpa/inet.h>
#include<exception>
#include<algorithm>
#include<time.h>
#include<stdio.h>
#include<vector>
//���Ը���fd�ҵ�UserData��
//ͨ��UserData�ҵ�Timer�����Գ�Աindex��TimerHeap����Ľӿڶ�fd��Ӧ��Timer���в���
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

