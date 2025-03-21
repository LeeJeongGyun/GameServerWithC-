#pragma once
#include "Allocator.h"

#define OUT

// Lock Macro
#define USE_MANY_LOCKS(count)   Lock _locks[count]
#define USE_LOCK                USE_MANY_LOCKS(1)
#define READ_LOCK_IDX(idx)      ReadLockGuard readLockGuard_##idx(_locks[idx], typeid(this).name())
#define READ_LOCK               READ_LOCK_IDX(0)
#define WRITE_LOCK_IDX(idx)     WriteLockGuard writeLockGuard_##idx(_locks[idx], typeid(this).name())
#define WRITE_LOCK              WRITE_LOCK_IDX(0)


// CRASH

#define CRASH(cause)                    \
{                                       \
    uint32 *ptr = nullptr;              \
    __analysis_assume(ptr != nullptr);  \
    *ptr = 0xAAAAAAAA;                  \
}

#define ASSERT_CRASH(expr)              \
{                                       \
    if (!(expr))                        \
    {                                   \
        CRASH("ASSERT_CRASH");          \
        __analysis_assume(expr);        \
    }                                   \
}

// Memory Allocation
#if _DEBUG
    #define XAlloc(size)    StompAllocator::Alloc(size)
    #define XFree(ptr)      StompAllocator::Free(ptr)
#else
    #define XAlloc(size)    BaseAllocator::Alloc(size)
    #define XFree(ptr)      BaseAllocator::Free(ptr)
#endif
