#pragma once

/**
 * @class BaseAllocator
 * @brief 기본 메모리 할당자
 */
class BaseAllocator
{
public:
    static void* Alloc(uint32 size);
    static void Free(void* ptr);
};

/**
 * @class StompAllocator
 * @brief 메모리 버그를 잡기 위한 디버깅 용 메모리 할당자
 */
class StompAllocator
{
    enum
    {
        PAGE_SIZE = 4096,
    };

public:
    static void* Alloc(uint32 size);
    static void Free(void* ptr);
};
