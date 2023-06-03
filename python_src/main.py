from drone import Drone
from car import Car
from messagequeue import mq_init, pop
import time

def init(drone):
    drone.connect_to_pixhawk()
    #drone.sim_connect_to_pixhawk()
    drone.arm_and_takeoff_to_pixhawk(3.0)

def update(msg, roadMap, car, drone, pre_car_road_id):

    drone.update_drone_data()

    pre_car_road_id = car.road_id

    car.update_car_data(msg)

    # car pre road is not match update will go waypoint update
    if pre_car_road_id != car.road_id:
        waypoints = roadMap[car.road_id]
        drone.update_will_go_waypoint(waypoints)
        drone.add_waypoints_to_pixhawk(waypoints)
    
    drone.update_drone_target()

    car_data = car.get_car_data()
    drone.update_drone_velocity(car_data)
    
    drone.set_airspeed_to_pixhawk(drone.velocity)

    return pre_car_road_id

def init_make_logfile():
    with open("logfile.txt", "w") as file:
        file.write("time,car_speed,car_road_id,car_waypoint_id,drone_target_speed,drone_speed,drone_road_id,drone_waypoint_id\n")

def add_logfile(data):
    with open("logfile.txt", "a") as file:
        file.write(data)

def flight_control(a):
    # Get message queue ID using same key as C++ program
    key = 5656
    mq = mq_init(key)
    starttime = time.time()


    roadMap = {1: [[1, 1, 35.1554345, 128.1040969, 3.0], [1, 2, 35.1553926, 128.1041161, 3.0], [1, 3, 35.1553504, 128.1041358, 3.0], [1, 4, 35.1553084, 128.104155, 3.0], [1, 5, 35.1552665, 128.1041744, 3.0], [1, 6, 35.155224, 128.1041938, 3.0], [1, 7, 35.1551821, 
    128.1042133, 3.0], [1, 8, 35.1551398, 128.1042324, 3.0], [1, 9, 35.1550982, 128.1042529, 3.0], [1, 10, 35.155056, 128.1042726, 3.0], [1, 11, 35.155014, 128.1042911, 3.0], [1, 12, 35.1549721, 128.1043098, 3.0], [1, 13, 35.1549304, 128.1043303, 3.0], [1, 14, 35.1548882, 128.1043484, 3.0], [1, 15, 35.1548469, 128.1043689, 3.0], [1, 16, 35.1548046, 128.1043888, 3.0], [1, 17, 35.1547624, 128.1044084, 3.0], [1, 18, 35.1547202, 128.1044262, 3.0], [1, 19, 35.1546788, 128.1044466, 3.0], [1, 20, 35.1546363, 128.1044658, 3.0], [1, 21, 35.1545946, 128.1044852, 3.0], [1, 22, 35.1545524, 128.104505, 3.0], [1, 23, 35.1545105, 128.1045241, 3.0], [1, 24, 35.1544685, 128.1045432, 3.0], [1, 25, 35.1544269, 128.1045633, 3.0], [1, 26, 35.1543852, 128.1045824, 3.0]],
    2: [[2, 1, 35.1543528, 128.1045948, 3.0], [2, 2, 35.1543109, 128.1046136, 3.0], [2, 3, 35.154269, 128.1046327, 3.0], [2, 4, 35.1542267, 128.1046525, 3.0], [2, 5, 35.1541848, 128.1046719, 3.0], [2, 6, 35.1541423, 128.1046911, 3.0], [2, 7, 35.1541135, 128.1047038, 3.0]],
    3: [[3, 1, 35.1543841, 128.1046156, 3.0], [3, 2, 35.1543956, 128.1046688, 3.0], [3, 3, 35.1544071, 128.1047219, 3.0], [3, 4, 35.1544186, 128.1047751, 3.0], [3, 5, 35.1544301, 128.1048282, 3.0]],
    4: [[4, 1, 35.1543704, 128.1045506, 3.0], [4, 2, 35.1543591, 128.1044976, 3.0], [4, 3, 35.1543482, 128.1044443, 3.0], [4, 4, 35.1543367, 128.1043907, 3.0], [4, 5, 35.1543257, 128.1043377, 3.0], [4, 6, 35.1543142, 128.104284, 3.0], [4, 7, 35.1543032, 128.1042311, 3.0], [4, 8, 35.1542914, 128.1041778, 3.0]],
    5: [[5, 1, 35.1544608, 128.10485, 3.0], [5, 2, 35.1545011, 128.1048254, 3.0], [5, 3, 35.1545413, 128.1048007, 3.0], [5, 4, 35.1545816, 128.1047761, 3.0], [5, 5, 35.1546218, 128.1047514, 3.0], [5, 6, 35.1546621, 128.1047268, 3.0], [5, 7, 35.1547023, 128.1047021, 3.0], [5, 8, 35.1547425, 128.1046775, 3.0], [5, 9, 35.1547827, 128.1046528, 3.0]],
    6: [[6, 1, 35.1544513, 128.1048989, 3.0], [6, 2, 35.1544656, 128.1049509, 3.0], [6, 3, 35.1544798, 128.1050029, 3.0]],
    7: [[7, 1, 35.1544175, 128.1048654, 3.0], [7, 2, 35.1543759, 128.1048862, 3.0], [7, 3, 35.1543342, 128.104907, 3.0], [7, 4, 35.1542926, 128.1049278, 3.0], [7, 5, 35.1542509, 128.1049486, 3.0]]
    }

    drone = Drone()
    car = Car()
    pre_car_road_id = car.road_id

    init_make_logfile()
    init(drone)
    
    while True:
        msg = pop(mq)
    
        pre_car_road_id = update(msg, roadMap, car, drone, pre_car_road_id)
        
        update_time = time.time()
        
        log_data = "{},{},{},{},{},{},{},{}\n".format(update_time-starttime,car.velocity,car.road_id,car.waypoint,drone.velocity,drone.current_speed,drone.road_id,drone.waypoint)

        add_logfile(log_data)
