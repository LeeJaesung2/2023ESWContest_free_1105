#include "../../include/speed_control.h"

void * thread_func1(void *arg)
{
    int i = 0;
    while(i<1000){
        i++;
        printf("A");
    }
    return 0;
}