#include "pch.h"
#include "Allocator.h"

/**
 * @details malloc�� �̿��Ͽ� �޸𸮸� �Ҵ��մϴ�.
 * @param[in] size ��û�� �޸� ũ���Դϴ�.
 * @return void*
 * �Ҵ�� �޸� �ּҸ� �����մϴ�.
 */
void* BaseAllocator::Alloc(uint32 size)
{
    return ::malloc(size);
}

/**
 * @details ���ڷ� ���� �����Ͱ� ����Ű�� �޸� �ּҸ� �����մϴ�.
 * @param[in] �����Ǳ� ���ϴ� �ּ��Դϴ�.
 * @return void
 */
void BaseAllocator::Free(void* ptr)
{
    ::free(ptr);
}

/**
 * @details VirtualAlloc �Լ��� �̿��Ͽ� �޸𸮸� �Ҵ��մϴ�.
 * Use After Free & �޸� Overflow ��Ȳ�� �ٷ� ��� ���� ����� ��忡���� ���˴ϴ�.
 * ��û ���� �޸� ũ��� �޸� ��ġ�� �����ؼ� �����մϴ�.
 * @param[in] size ��û�� �޸� ũ���Դϴ�.
 * @return void*
 * ������ �޸� �ּҸ� �����մϴ�.
 */
void* StompAllocator::Alloc(uint32 size)
{
    const int32 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    const int32 memoryOffset = pageCount * PAGE_SIZE - size;
    void* baseAddress = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    return static_cast<void*>(static_cast<int8*>(baseAddress) + memoryOffset);
}

/**
 * @details ���ڷ� ���� �����Ͱ� ����Ű�� �޸� �ּҸ� �����մϴ�.
 * @param[in] �����Ǳ� ���ϴ� �ּ��Դϴ�.
 * @return void
 */
void StompAllocator::Free(void* ptr)
{
    const int64 curAddress = reinterpret_cast<uint64>(ptr);
    const int64 baseAddress = (curAddress / PAGE_SIZE) * PAGE_SIZE;
    ::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);
}