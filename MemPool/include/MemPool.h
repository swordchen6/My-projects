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
//block�õ�������֯����
//����ʱ�ҵ���Ӧ��page_id�ٲ��뵽��Ӧ��page����
typedef struct MemBlock
{
	int					page_id;				//MemPool��Ӧ��page_id
	void*				mem_ptr;				//block�������ݵ�ָ��
	struct MemBlock*			next_block;				//ָ����һ��block��ָ��
}MemBlock;
//block����ʱ����ͷ��������ʹ��ʱ����β��
typedef struct MemPage
{
	MemBlock*			blocks;					//blocks������
	int					unused;					//block�ѱ�ʹ������
}MemPage;

typedef struct MemPool
{
	MemPage**			pages;					//pages����
	int					pages_num;				//pages����
#if EBABLE_THREAD_SAFE
	pthread_mutex_t		pool_mutex;				//�ڴ�ػ�����
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