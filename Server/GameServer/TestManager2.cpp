#include "pch.h"
#include "TestManager2.h"
#include "TestManager1.h"

TestManager2 GTestManager2;

void TestManager2::Lock()
{
	WRITE_LOCK;
}

void TestManager2::AA()
{
    WRITE_LOCK;
    std::cout << "TestManager2::Lock()" << std::endl;

    GTestManager1.Lock();
}
