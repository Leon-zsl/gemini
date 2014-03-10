#ifndef __Gemini_BlockQueue__
#define __Gemini_BlockQueue__

#include <queue>

#include "Lock.h"

namespace base
{
template <class _Ty>
class BlockQueue
{
Public:
    BlockQueue();
    ~BlockQueue();

    void Push(_Ty item)
    {
        _lock.lock();
        queue.push(item);
        _lock.unlock();
    }

    _Ty Pop(void)
    {
        _lock.lock();
        _Ty item = queue.pop();
        _lock.unlock();
        return item;
    }

    int Count(void)
    {
        _lock.lock();
        int size = (int)queue.size();
        _lock.unlock();
        return size;
    }

    bool Empty(void)
    {
        _lock.lock();
        bool emp = queue.empty();
        _lock.unlock();        
        return emp;
    }

private:
    Lock _lock;
    std::queue _queue;
};
}
#endif
