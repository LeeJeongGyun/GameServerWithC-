#pragma once

#define CRASH(cause) \
{                           \
    uint32 *ptr = nullptr;  \
    __analysis_assume(ptr != nullptr); \
    *ptr = 0xAAAAAAAA;      \
}

#define ASSERT_CRASH(expr)      \
{                               \
    if (!(expr))                \
    {                           \
        CRASH("ASSERT_CRASH");  \
        __analysis_assume(expr);    \
    }                           \
}
