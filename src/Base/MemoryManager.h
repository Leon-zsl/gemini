#ifndef __Example__MemoryManager__
#define __Example__MemoryManager__

#include <stdlib.h>

namespace base
{
template <class Ty_Allocator>
class MemoryManager
{
public:
    void* operator new(size_t size) { return alloc.Allocate(size); }
    void operator delete(void* p, size_t size) { return alloc.Deallocate(p, size); }

    void* operator new[] (size_t size) { return alloc.Allocate(size); }
    void operator delete[] (void* p, size_t size) { return alloc.Deallocate(p, size); }

public:
    static Ty_Allocator alloc;
};
}
#endif
