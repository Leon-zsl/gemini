//#include "UniformAllocator.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory>

namespace base
{
template <class _Ty>
typename UniformAllocator<_Ty>::Chunk* UniformAllocator<_Ty>::_free_list = NULL;

template <class _Ty>
Lock UniformAllocator<_Ty>::_lock;

template <class _Ty>
void* UniformAllocator<_Ty>::Allocate(size_t size)
{
    if(size == 0) return NULL;
    if(size < 4) size = 4;
    if(size > 128) return malloc(size);
    
    _lock.lock();
    if(_free_list == NULL) {
        _free_list = fetch_and_alloc_memory(size, 8);
    }

    Chunk* p = _free_list;
    _free_list = _free_list->next;
    _lock.unlock();
    return p;
}

template <class _Ty>
void UniformAllocator<_Ty>::Deallocate(void* p, size_t size)
{
    if(size == 0 || p == NULL) return;
    if(size > 128) { free(p); return; }

    _lock.lock();
    ((Chunk*)p)->next = _free_list;
    _free_list = (Chunk*)p;
    _lock.unlock();
}

template <class _Ty>
typename UniformAllocator<_Ty>::Chunk* UniformAllocator<_Ty>::fetch_and_alloc_memory(size_t size, int n)
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

