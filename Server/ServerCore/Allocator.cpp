#include "pch.h"
#include "Allocator.h"

/**
 * @details malloc을 이용하여 메모리를 할당합니다.
 * @param[in] size 요청된 메모리 크기입니다.
 * @return void*
 * 할당된 메모리 주소를 리턴합니다.
 */
void* BaseAllocator::Alloc(uint32 size)
{
    return ::malloc(size);
}

/**
 * @details 인자로 들어온 포인터가 가리키는 메모리 주소를 해제합니다.
 * @param[in] 해제되길 원하는 주소입니다.
 * @return void
 */
void BaseAllocator::Free(void* ptr)
{
    ::free(ptr);
}

/**
 * @details VirtualAlloc 함수를 이용하여 메모리를 할당합니다.
 * Use After Free & 메모리 Overflow 상황을 바로 잡기 위해 디버깅 모드에서만 사용됩니다.
 * 요청 받은 메모리 크기와 메모리 위치를 수정해서 리턴합니다.
 * @param[in] size 요청된 메모리 크기입니다.
 * @return void*
 * 수정된 메모리 주소를 리턴합니다.
 */
void* StompAllocator::Alloc(uint32 size)
{
    const int32 pageCount = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    const int32 memoryOffset = pageCount * PAGE_SIZE - size;
    void* baseAddress = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    return static_cast<void*>(static_cast<int8*>(baseAddress) + memoryOffset);
}

/**
 * @details 인자로 들어온 포인터가 가리키는 메모리 주소를 해제합니다.
 * @param[in] 해제되길 원하는 주소입니다.
 * @return void
 */
void StompAllocator::Free(void* ptr)
{
    const int64 curAddress = reinterpret_cast<uint64>(ptr);
    const int64 baseAddress = (curAddress / PAGE_SIZE) * PAGE_SIZE;
    ::VirtualFree(reinterpret_cast<void*>(baseAddress), 0, MEM_RELEASE);
}