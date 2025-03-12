#pragma once

class BaseAllocator
{
public:
    static void* Alloc(uint32 size);
    static void Free(void* ptr);
};
