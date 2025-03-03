#include "pch.h"
#include <iostream>
using namespace std;

int main()
{
    for (int i = 0; i < 10; ++i)
    {
        GThreadManager->Launch([]() {
            while (true)
            {
                printf("ThreadId: %d\n", LThreadId);
                this_thread::sleep_for(1000ms);
            }
        });
    }

    GThreadManager->Join();
}
