#pragma once

#define OUT

// Lock Macro
#define USE_MANY_LOCKS(count)   Lock _locks[count]
#define USE_LOCK                USE_MANY_LOCKS(1)
#define READ_LOCK_IDX(idx)      ReadLockGuard readLockGuard_##idx(_locks[idx])
#define READ_LOCK               READ_LOCK_IDX(0)
#define WRITE_LOCK_IDX(idx)     WriteLockGuard writeLockGuard_##idx(_locks[idx])
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

