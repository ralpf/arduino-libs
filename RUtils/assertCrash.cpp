#include "assertCrash.h"
#include "uniPrinter.h"


#define ENABLE_ASSERT


#ifdef ENABLE_ASSERT
void assert_fail(const char* msg)
{
    SPrint("[ASSERT FAILED] %s", msg);
    //while (1) {}   // infinite loop or do esp_system_abort()
}
#else
void assert_fail(const char* msg) { }
#endif