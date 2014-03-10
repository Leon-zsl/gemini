#ifndef __Gemini__GlobalAllocator__
#define __Gemini__GlobalAllocator__

#include "Lock.h"

namespace base
{
class GlobalAllocator
{
public:
    void* Allocate(size_t size);
    void Deallocate(void* p, size_t size);

private:
    typedef union _Chunk {
        union _Chunk* next;
        char block;
    } Chunk;

    Chunk* fetch_and_alloc_memory(size_t size, int n);
    int get_free_list_index_by_size(size_t n);
    size_t ceil_size(size_t size);

private:
    static Lock _lock;
    static Chunk* _free_list_array[6];
};
}
#endif
