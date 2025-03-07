#pragma once
class TestManager1;

class TestManager2
{
    USE_LOCK;
public:
    void Lock();
    void AA();

private:
};

extern TestManager2 GTestManager2;