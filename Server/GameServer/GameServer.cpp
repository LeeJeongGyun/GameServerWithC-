#include "pch.h"
#include "Allocator.h"
using namespace std;

class Test
{
public:
    int hp;
    int mp;
};

class Knight : public Test
{
public:
    int attack;
};

int main()
{
    Test* ptr = XNew<Test>();
    
    Knight* ptr2 = (Knight*)ptr;
    ptr2->attack = 100;

    XDelete(ptr);
}
