#pragma once

/**
 * @class BaseAllocator
 * @brief �⺻ �޸� �Ҵ���
 */
class BaseAllocator
{
public:
    static void* Alloc(uint32 size);
    static void Free(void* ptr);
};

/**
 * @class StompAllocator
 * @brief �޸� ���׸� ��� ���� ����� �� �޸� �Ҵ���
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
