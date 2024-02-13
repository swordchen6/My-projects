#include <stdio.h>
#include"ThreadPool.h"
void func(void* arg)
{
    printf("thread %ld is working,num is %d\n", pthread_self(), *(int*)arg);
}
void test()
{
    //创建线程池
    ThreadPool* pool = threadPoolCreate(3, 10, 100);
    for (int i = 0; i < 100; i++)
    {
        int* num = (int*)malloc(sizeof(int));
        *num = i + 100;
        Task* obj=(Task*)malloc(sizeof(Task));
        obj->function = func;
        obj->arg = num; 
        addTask(pool, obj);
    }
    sleep(5);
    threadPoolDestroy(pool);
}
int main()
{
    test();
    return 0;
}
