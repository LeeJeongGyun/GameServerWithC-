#include "pch.h"
#include "TestManager1.h"
#include "TestManager2.h"

TestManager1 GTestManager1;

void TestManager1::Lock()
{
    WRITE_LOCK;
}

void TestManager1::AA()
{
    WRITE_LOCK;
    std::cout << "TestManager1::Lock()" << std::endl;
    GTestManager2.Lock();
}
