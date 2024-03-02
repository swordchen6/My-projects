#ifndef	_MEM_POOL_H_
#define	_MEM_POOL_H_
#include <stdlib.h>
#define EBABLE_THREAD_SAFE 1

#if EBABLE_THREAD_SAFE
#include<pthread.h>
#endif

#define BLOCK_SIZE			32
#define PAGE_SIZE			4096
#define INIT_PAGE_NUM		10
//block用单链表组织起来
//回收时找到对应的page_id再插入到对应的page就行
typedef struct MemBlock
{
	int					page_id;				//MemPool对应的page_id
	void*				mem_ptr;				//block保存数据的指针
	struct MemBlock*			next_block;				//指向下一个block的指针
}MemBlock;
//block回收时插入头部，当被使用时插入尾部
typedef struct MemPage
{
	MemBlock*			blocks;					//blocks单链表
	int					unused;					//block已被使用数量
}MemPage;

typedef struct MemPool
{
	MemPage**			pages;					//pages数组
	int					pages_num;				//pages数量
#if EBABLE_THREAD_SAFE
	pthread_mutex_t		pool_mutex;				//内存池互斥锁
#endif

}MemPool;
//API
int mem_block_init(MemBlock* block, int id);
void mem_block_destroy(MemBlock* block);
void mem_page_destroy(MemPage* page);
int mem_page_init(MemPage* page, int id);
void mem_pool_destroy(MemPool* pool);
int mem_pool_init(MemPool* pool);
MemBlock* mem_malloc(MemPool* pool, int num);
void mem_free(MemPool* pool, MemBlock* head, int num);
#endif // _MEM_POOL_H_