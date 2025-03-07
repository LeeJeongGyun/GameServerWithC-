#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"

void Lock::WriteLock(const char *name)
{
#if _DEBUG
    GDeadLockProfiler->PushLock(name);
#endif

    uint32 curThreadId = (_lockFlag & WRITE_THREAD_MASK) >> 16;
    if (curThreadId == LThreadId)
    {
        _writeCount++;
        return;
    }

    const uint64 beginTick = ::GetTickCount64();
    while (true)
    {
        uint32 desired = (LThreadId << 16) & WRITE_THREAD_MASK;
        for (int32 spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount)
        {
            uint32 expected = EMPTY_FLAG;
            if (_lockFlag.compare_exchange_strong(OUT expected, desired))
            {
                _writeCount++;
                return;
            }
        }

        if (::GetTickCount64() - beginTick > ACQUIRE_TIMEOUT_TICK)
            CRASH("LOCK TIMEOUT");

        _mm_pause(); // YieldProcessor();
    }
}

void Lock::WriteUnlock(const char *name)
{
#if _DEBUG
    GDeadLockProfiler->PopLock(name);
#endif

    uint32 curThreadId = (_lockFlag & WRITE_THREAD_MASK) >> 16;
    if (curThreadId != LThreadId)
        CRASH("INVALID WRITE UNLOCK");

    // R Lock 전부 풀기전에 W Lock 불가능
    if ((_lockFlag & READ_COUNT_MASK) != 0)
        CRASH("INVALID UNLOCK ORDER");

    if (--_writeCount != 0)
        return;

    _lockFlag = EMPTY_FLAG;
}

void Lock::ReadLock(const char *name)
{
#if _DEBUG
    GDeadLockProfiler->PushLock(name);
#endif

    uint32 curThreadId = (_lockFlag & WRITE_THREAD_MASK) >> 16;
    if (curThreadId == LThreadId)
    {
        _lockFlag.fetch_add(1);
        return;
    }

    const uint64 beginTick = ::GetTickCount64();
    while (true)
    {
        for (int spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount)
        {
            uint32 expected = _lockFlag & READ_COUNT_MASK;
            if (_lockFlag.compare_exchange_strong(OUT expected, expected + 1))
                return;
        }

        if (::GetTickCount64() - beginTick > ACQUIRE_TIMEOUT_TICK)
            CRASH("LOCK TIMEOUT");

        _mm_pause();
    }
}

void Lock::ReadUnlock(const char *name)
{
#if _DEBUG
    GDeadLockProfiler->PopLock(name);
#endif

    if (0 == (_lockFlag.fetch_sub(1) & READ_COUNT_MASK))
        CRASH("INVALID READ UNLOCK");
}
