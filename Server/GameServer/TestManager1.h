#pragma once

class TestManager2;

class TestManager1
{
    USE_LOCK;

public:
    void Lock();
    void AA();

public:

};

extern TestManager1 GTestManager1;