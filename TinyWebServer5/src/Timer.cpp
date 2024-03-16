#include "Timer.h"

TimerHeap::TimerHeap(int pMaxSize)
{
	maxSize = pMaxSize;
	curSize = 0;
	heap = new Timer[pMaxSize];
	if (!heap)
	{
		throw std::exception();
	}
}

TimerHeap::~TimerHeap()
{
	if (heap)
	{
		delete[]heap;
	}
}

bool TimerHeap::setTimer(int fd, sockaddr_in addr, UserData* ud)
{
	if (curSize >= maxSize||!ud)
	{
		return false;
	}
	ud->fd = fd;
	ud->addr = addr;
	ud->index = curSize;
	heap[curSize].data = ud;
	heap[curSize].expire = time(NULL) + 3 * TIMESLOT;
	curSize++;
	return true;
}

int TimerHeap::delTimer(int index)
{
	if (index<0||index >= curSize||!heap[index].data)
	{
		return -1;
	}

	int ret = heap[index].data->fd;

	std::swap(heap[index], heap[curSize - 1]);
	heap[index].data->index = index;
	curSize--;

	downwardAjust(index);
	return ret;
}

bool TimerHeap::adjustTimer(int index)
{
	if (index<0||index >= curSize || !heap[index].data)
	{
		return false;
	}
	std::swap(heap[index], heap[curSize - 1]);
	heap[curSize - 1].data->index = curSize - 1;
	heap[index].data->index = index;
	heap[curSize - 1].expire = time(NULL) + 3 * TIMESLOT;
	downwardAjust(index);
	return true;
}

std::vector<int> TimerHeap::tick()
{
	std::vector<int> ans;
	time_t now = time(NULL);
	while (curSize > 0 && heap[0].expire <= now)
	{
		ans.push_back(delTimer(0));
	}
	return ans;
}

void TimerHeap::upwardAdjust(int index)
{
	if (index <= 0)
		return;
	
	while (index > 0)
	{
		int parentIndex = (index - 1) / 2;
		if (heap[parentIndex].expire > heap[index].expire)
		{
			//heap[index]的成员为expire和指针data，因此默认的‘=’号可以使用
			std::swap(heap[parentIndex], heap[index]);
			//更新index值
			heap[index].data->index = index;
			heap[parentIndex].data->index = parentIndex;
			//更新index做下次循环
			index = parentIndex;
		}
		else
		{
			break;
		}
	}
}

void TimerHeap::downwardAjust(int index)
{
	while (2 * index + 1 < curSize)
	{
		if (2 * index + 2 >= curSize)
		{
			if (heap[index].expire > heap[2 * index + 1].expire)
			{
				std::swap(heap[index], heap[2 * index + 1]);
				heap[index].data->index = index;
				heap[2 * index + 1].data->index = 2 * index + 1;
				index = 2 * index + 1;
			}
			else
			{
				break;
			}
		}
		else
		{
			if (heap[2 * index + 1].expire <= heap[2 * index + 2].expire)
			{
				if (heap[index].expire > heap[2 * index + 1].expire)
				{
					std::swap(heap[index], heap[2 * index + 1]);
					heap[index].data->index = index;
					heap[2 * index + 1].data->index = 2 * index + 1;
					index = 2 * index + 1;
				}
				else
				{
					break;
				}
			}
			else
			{
				if (heap[index].expire > heap[2 * index + 2].expire)
				{
					std::swap(heap[index], heap[2 * index + 2]);
					heap[index].data->index = index;
					heap[2 * index + 2].data->index = 2 * index + 2;
					index = 2 * index + 2;
				}
				else
				{
					break;
				}
			}
		}
	}
}
