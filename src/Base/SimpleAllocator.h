#ifndef __Gemini_SimpleAllocator__
#define __Gemini_SimpleAllocator__

#include <stdlib.h>

namespace base
{
class SimpleAllocator
{
public:
    void* Allocate(size_t size) { return malloc(size); }
    void Deallocate(void* p, size_t size) { free(p); }
};
}
#endif
