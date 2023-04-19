#ifndef __VISIBILITY_H__
#define __VISIBILITY_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <tbb/concurrent_queue.h>


struct carData{
    int road_id;
    int waypoint_id;
    float velocity;
};

struct droneData{
    int road_id;
    int waypoint_id;
    float velocity;
    float will_go_waypoint[100];
};


#endif

