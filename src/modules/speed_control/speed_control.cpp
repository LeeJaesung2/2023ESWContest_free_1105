#include <DBASEV/speed_control.h>

void * thread_func1(void *arg)
{
    const char* src = "speed_control";
    const char* func = "speed_control";
    embedd(src, func, 3, 7);   
    return 0;
}