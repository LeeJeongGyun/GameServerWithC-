#include "pch.h"
#include <iostream>
using namespace std;

#include "TestManager1.h"
#include "TestManager2.h"

int main()
{
    TestManager1 *ptr1 = new TestManager1();
    TestManager2 *ptr2 = new TestManager2();
    GThreadManager->Launch([&]() {
        while (true)
        {
            ptr1->AA();
            //this_thread::sleep_for(10ms);
        }
    });

    GThreadManager->Launch([&]() {
        while (true)
        {
            ptr2->AA();
            this_thread::sleep_for(10ms);
        }
    });

    GThreadManager->Join();
}
