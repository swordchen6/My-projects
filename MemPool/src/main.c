#include <stdio.h>
#include"MemPool.h"
void test1()
{
    MemPool* pool = (MemPool*)malloc(sizeof(MemPool));
    if (!pool)
    {
        return;
    }
    mem_pool_init(pool);
    for (int i = 0; i < 100; i++)
    {
        for (int j = i + 1;j>0;j--)
        {
            MemBlock* blocks = mem_malloc(pool, j);
            mem_free(pool, blocks, j);
        }
    }
}
int main()
{
    test1();
    return 0;
}