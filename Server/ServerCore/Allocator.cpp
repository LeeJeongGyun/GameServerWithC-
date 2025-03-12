#include "pch.h"
#include "Allocator.h"

void* BaseAllocator::Alloc(uint32 size)
{
    return ::malloc(size);
}

void BaseAllocator::Free(void* ptr)
{
    ::free(ptr);
}
