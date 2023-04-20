#include <DBASEV/speed_control.h>
#include <DBASEV/embedd.h>
void * thread_func1(void *arg)
{   
    // tbb::concurrent_queue<int>* cq = static_cast<tbb::concurrent_queue<int>*>(arg);
    // int item;
    // for (int i = 0; i < 15; i++) {
    //     if (cq->try_pop(item)) { // try to pop item from the concurrent queue
    //         printf("Popped item %d\n", item);
    //     } else {
    //         printf("Concurrent queue is empty\n");
    //     }
    // }
    const char* src = "speed_control";
    const char* func = "speed_control";
    const char* msg = "struct argument test success";
    carData car_data = {1, 55, 87.4};
    waypoint point1 = {1,10.0,5.2,8.6,6,1};
    waypoint point2 = {3,2.0,7.2,9.6,8,0};
    waypoint point3 = {3,2.0,7.2,9.6,8,0};
    droneData drone_data = {5, 8, 60.8, {point1,point2, point3}};
    //callPython(src, func, 1, 1);   
    callPythonStruct(src, func, msg, car_data, drone_data);
    return 0;
}