#include "GlobalAllocator.h"

#include <stdlib.h>
#include <stdio.h>
#include <memory>

namespace base
{
GlobalAllocator::Chunk* GlobalAllocator::_free_list_array[6] = { NULL, NULL, NULL, NULL, NULL, NULL };
Lock GlobalAllocator::_lock;

void* GlobalAllocator::Allocate(size_t size)
{
    if(size == 0) return NULL;

    if(size > 128) return malloc(size);
    
    int idx = get_free_list_index_by_size(size);
    if(idx == -1) return malloc(size);

    _lock.lock();
    Chunk* bucket = _free_list_array[idx];
    if(bucket == NULL) {
        size_t s = ceil_size(size);
        Chunk* p = fetch_and_alloc_memory(s, 8);
        _free_list_array[idx] = p;
        bucket = p;
    }
    _free_list_array[idx] = bucket->next;
    _lock.unlock();
    return bucket;
}

void GlobalAllocator::Deallocate(void* p, size_t size)
{
    if(p == NULL || size == 0) return;

    if(size > 128) { free(p); return; }

    int idx = get_free_list_index_by_size(size);
    if(idx == -1) { free(p); return; }

    _lock.lock();
    Chunk* bucket = _free_list_array[idx];
    ((Chunk*)p)->next = bucket;
    _free_list_array[idx] = (Chunk*)p;
    _lock.unlock();
}

int GlobalAllocator::get_free_list_index_by_size(size_t n)
{
    if(n <= 4) return 0;
    if(n <= 8) return 1;
    if(n <= 16) return 2;
    if(n <= 32) return 3;
    if(n <= 64) return 4;
    if(n <= 128) return 5;

    return -1;
}

size_t GlobalAllocator::ceil_size(size_t size)
{
    if(size <= 4) return 4;
    if(size <= 8) return 8;
    if(size <= 16) return 16;
    if(size <= 32) return 32;
    if(size <= 64) return 64;
    if(size <= 128) return 128;
    
    return size;
}

GlobalAllocator::Chunk* GlobalAllocator::fetch_and_alloc_memory(size_t size, int n)
{
    char* p = (char*)malloc(size * n);
    if(p == NULL) {
        printf("fetch new memory failed: %d", (int)size * n);
        return (Chunk*)p;
    }

    memset(p, 0, size * n);

    int idx = 0;
    while(idx++ < n) {
        ((Chunk*)p)->next = (Chunk*)(p + size);
        p = p + size;
    }
    return (Chunk*)p;
}
}