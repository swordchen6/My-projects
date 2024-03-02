#include"MemPool.h"
int mem_block_init(MemBlock* block,int id)
{
	if (!block)
	{
		return -1;
	}
	block->next_block = NULL;
	block->page_id = id;
	block->mem_ptr = (void*)malloc(BLOCK_SIZE);
	if (!block->mem_ptr)
	{
		return -1;
	}
	return 0;
}
void mem_block_destroy(MemBlock* block)
{
	if (!block)
	{
		return;
	}
	if(block->mem_ptr)
		free(block->mem_ptr);
	block->next_block = NULL;
	return;
}
void mem_page_destroy(MemPage* page)
{
	if (!page)
	{
		return;
	}
	while (page->blocks)
	{
		MemBlock* p = page->blocks;
		page->blocks = p->next_block;
		mem_block_destroy(p);
		free(p);
	}
	return;
}

int mem_page_init(MemPage* page,int id)
{
	do
	{
		if (!page)
		{
			break;
		}
		page->unused = PAGE_SIZE / BLOCK_SIZE;
		page->blocks = NULL;
		int flag = 0;
		for (int i = 0; i < page->unused; i++)
		{
			MemBlock* p = (MemBlock*)malloc(sizeof(MemBlock));
			if (!p)
			{
				flag = 1;
				break;
			}
			int ret = mem_block_init(p, id);
			if (ret == -1)
			{
				flag = 1;
				free(p);
				break;
			}
		}
		if (flag)
		{
			break;
		}
		return 0;
	} while (0);
	mem_page_destroy(page);
	return -1;
}

void mem_pool_destroy(MemPool* pool)
{
	if (!pool)
	{
		return;
	}
#if EBABLE_THREAD_SAFE
	pthread_mutex_destroy(&pool->pool_mutex);
#endif
	for (int i = 0; i < pool->pages_num; i++)
	{
		mem_page_destroy(pool->pages[i]);
		free(pool->pages[i]);
	}
	return;
}

int mem_pool_init(MemPool* pool)
{
	//判断pool是否指向有效的内存地址
	do
	{
		//pool部分
		if (!pool)
		{
			break;
		}
		pool->pages_num = INIT_PAGE_NUM;
		pool->pages = (MemPage**)malloc(INIT_PAGE_NUM*sizeof(MemPage*));
		if (!pool->pages)
		{
			break;
		}
#if EBABLE_THREAD_SAFE
		if (pthread_mutex_init(&pool->pool_mutex, NULL) != 0)
		{
			break;
		}
#endif
		//pages部分
		int flag = 0;
		for (int i = 0; i < pool->pages_num; i++)
		{
			pool->pages[i] = (MemPage*)malloc(sizeof(MemPage));
			if (!pool->pages[i])
			{
				flag = 1;
				break;
			}
			int ret=mem_page_init(pool->pages[i],i);
			if (ret == -1)
			{
				flag = 1;
				free(pool->pages[i]);
				break;
			}
		}
		if (flag)
		{
			break;
		}
		return 0;
	} while (0);
	mem_pool_destroy(pool);
	return -1;
}

MemBlock* mem_malloc(MemPool* pool,int num)
{
	if (!pool || !pool->pages||!pool->pages[0]->blocks)
		return NULL;
	for (int i = 0; i < pool->pages_num; i++)
	{
		if (pool->pages[i]->unused >= num)
		{
			MemBlock* p = pool->pages[i]->blocks;
			while (num--)
			{
				p = p->next_block;
			}
			MemBlock* q = pool->pages[i]->blocks;
#if EBABLE_THREAD_SAFE
			pthread_mutex_lock(&pool->pool_mutex);
			pool->pages[i]->blocks = p->next_block;
			pthread_mutex_unlock(&pool->pool_mutex);
#else
			pool->pages[i]->blocks = p->next_block;
#endif
			p->next_block = NULL;
			return q;
		}
	}
	return NULL;
}
void mem_free(MemPool* pool,MemBlock* head, int num)
{
	if (!pool || !pool->pages || !pool->pages[0]->blocks||!head)
		return ;
	MemBlock* p = head;
	while (p->next_block)
	{
		p = p->next_block;
	}
#if EBABLE_THREAD_SAFE
	pthread_mutex_lock(&pool->pool_mutex);
	p->next_block = pool->pages[p->page_id]->blocks;
	pool->pages[p->page_id]->blocks = head;
	pthread_mutex_unlock(&pool->pool_mutex);
#else
	p->next_block = pool->pages[p->page_id]->blocks;
	pool->pages[p->page_id]->blocks = head;
#endif
	return;
}