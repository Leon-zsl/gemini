#ifndef __Example_UniformAllocator__
#define __Example_UniformAllocator__

#include "Lock.h"

namespace base
{
template <class _Ty>
class UniformAllocator
{
public:
    void* Allocate(size_t size);
    void Deallocate(void* p, size_t size);

private:
    typedef union _Chunk {
        union _Chunk* next;
        char block[1];
    } Chunk;

    Chunk* fetch_and_alloc_memory(size_t size, int n);

private:
    static Lock _lock;
    static Chunk* _free_list;
};
}
#include "UniformAllocator.hpp"

#endif
