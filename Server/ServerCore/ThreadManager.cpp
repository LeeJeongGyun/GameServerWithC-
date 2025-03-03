#include "pch.h"
#include "ThreadManager.h"

ThreadManager::ThreadManager()
{
    // Main Thread Init
    InitTLS();
}

ThreadManager::~ThreadManager()
{
    Join();
    _threads.clear();
}

void ThreadManager::Launch(std::function<void(void)> callback)
{
    LockGuard lockGuard(_lock);
    _threads.push_back(std::thread([=]() {
        InitTLS();
        callback();
        DestroyTLS();
    }));
}

void ThreadManager::Join()
{
    for (std::thread &t : _threads)
        if (t.joinable())
            t.join();
}

void ThreadManager::InitTLS()
{
    static Atomic<int> sThreadId = 1;
    LThreadId = sThreadId.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{
}
