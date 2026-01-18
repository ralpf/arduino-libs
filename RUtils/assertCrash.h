#pragma once
// enable disable asserts here
#define ENABLE_ASSERT



void assert_fail(const char* msg);


#ifdef ENABLE_ASSERT
    #define ASSERT(cond, msg) { if (!(cond)) assert_fail(msg); }
#else
    #define ASSERT(cond, msg) { }
#endif
