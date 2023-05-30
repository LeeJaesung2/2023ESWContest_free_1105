from dronekit import connect, VehicleMode, LocationGlobalRelative, Command
from pymavlink import mavutil
import time
import argparse 
import dronekit_sitl

'''
parser = argparse.ArgumentParser(description='Print out vehicle state information. Connects to SITL on local PC by default.')
parser.add_argument('--connect', 
                help="vehicle connection target string. If not specified, SITL automatically started and used.")
args = parser.parse_args()

connection_string = args.connect
sitl = None

#Start SITL if no connection string specified
if not connection_string:
    sitl = dronekit_sitl.start_default()
    connection_string = sitl.connection_string()

print("\nConnecting to vehicle on: %s" % connection_string)
vehicle = connect(connection_string, wait_ready=True)

vehicle.wait_ready('autopilot_version')
'''

connection_string = 'udp:127.0.0.1:1450'
vehicle = connect(connection_string, wait_ready=True)
print("Connecting to vehicle on : %s" %(connection_string, ))


# takeoff function
def arm_and_takeoff_to_pixhawk(aTargetAltitude):
    """
    Arms vehicle and fly to aTargetAltitude.
    """

    print("Basic pre-arm checks")
    # Don't try to arm until autopilot is ready
    while not vehicle.is_armable:
        print(" Waiting for vehicle to initialise...")
        time.sleep(1)

    print("Arming motors")
    # Copter should arm in GUIDED mode
    vehicle.mode = VehicleMode("GUIDED")
    vehicle.armed = True

    # Confirm vehicle armed before attempting to take off
    while not vehicle.armed:
        print(" Waiting for arming...")
        vehicle.mode = VehicleMode("GUIDED")
        vehicle.armed = True
        time.sleep(1)

    print("Taking off!")
    vehicle.simple_takeoff(aTargetAltitude)  # Take off to target altitude

    while True:
        print(" Altitude: ", vehicle.location.global_relative_frame.alt)
        if vehicle.location.global_relative_frame.alt >= aTargetAltitude * 0.95:
            print("Reached target altitude")
            break
        time.sleep(1)


def add_waypoints_to_pixhawk(waypoint_list):
    cmd = vehicle.commands
    cmd.download()
    cmd.wait_ready()
    cmd.clear()
    
    for waypoint in waypoint_list:
        print("wapoint : ", waypoint[2], waypoint[3], waypoint[4])
        wp = LocationGlobalRelative(waypoint[2], waypoint[3], waypoint[4])
        cmd.add(
            Command(0,0,0, 
            mavutil.mavlink.MAV_FRAME_GLOBAL_RELATIVE_ALT,
            mavutil.mavlink.MAV_CMD_NAV_WAYPOINT,
            0, 1, 0, 0, 0, 0,
            wp.lat, wp.lon, wp.alt
            )
        )    
        
    cmd.upload()

def print_current_gps():
    current_gps = vehicle.location.global_frame
    print("위도: %.7f" % current_gps.lat)
    print("경도: %.7f" % current_gps.lon)
    print("고도: %.1f" % current_gps.alt)
    

def set_airspeed_to_pixhawk(airspeed):
    vehicle.airspeed = airspeed


def land_to_pixhawk(vehicle):
    vehicle.mode = VehicleMode("LAND") 
    while not vehicle.mode.name=='LAND':
        print("Waiting for drone to land...")
        time.sleep(1)

    vehicle.armed = False 
    vehicle.close() 

"""------------------------------------------------------------------------------------"""
'''
# curve course
waypoints = {1: [[1, 1, 35.1535098, 128.1002861, 15.0], [1, 2, 35.1535017, 128.1003405, 15.0], [1, 3, 35.153494, 128.1003952, 15.0], [1, 4, 
35.1534847, 128.1004491, 15.0], [1, 5, 35.153477, 128.1005038, 15.0], [1, 6, 35.1534693, 128.1005578, 15.0], [1, 7, 35.1534608, 
128.1006118, 15.0], [1, 8, 35.1534523, 128.1006654, 15.0], [1, 9, 35.1534447, 128.1007197, 15.0], [1, 10, 35.1534351, 128.100773, 15.0], [1, 11, 35.1534271, 128.1008267, 15.0], [1, 12, 35.1534208, 128.1008813, 15.0], [1, 13, 35.1534126, 128.100935, 15.0], [1, 14, 35.1534038, 128.1009893, 15.0], [1, 15, 35.1533948, 128.1010436, 15.0], [1, 16, 35.1533868, 128.1010972, 15.0], [1, 17, 35.1533789, 128.1011509, 15.0], [1, 18, 35.1533701, 128.1012052, 15.0], [1, 19, 35.1533613, 128.1012592, 15.0], [1, 20, 35.1533517, 128.1013132, 15.0], [1, 21, 35.1533446, 128.1013675, 15.0], [1, 22, 35.1533369, 128.1014218, 15.0], [1, 23, 35.1533298, 128.1014758, 15.0], [1, 24, 35.153321, 128.1015294, 15.0], [1, 25, 35.1533134, 128.1015834, 15.0], [1, 26, 35.1533043, 128.1016374, 15.0], [1, 27, 35.1532961, 128.101691, 15.0], [1, 28, 35.1532879, 128.1017447, 15.0], [1, 29, 35.1532796, 128.1017993, 15.0], [1, 
30, 35.15327, 128.1018529, 15.0], [1, 31, 35.1532618, 128.1019066, 15.0], [1, 32, 35.1532544, 128.1019609, 15.0], [1, 33, 35.1532465, 128.1020152, 15.0], [1, 34, 35.1532388, 128.1020695, 15.0], [1, 35, 35.1532308, 128.1021232, 15.0], [1, 36, 35.1532218, 128.1021772, 15.0], [1, 37, 35.1532119, 128.1022452, 15.0]],
2: [[2, 38, 35.1532566, 128.1022536, 15.0], [2, 39, 35.153301, 128.102264, 15.0], [2, 40, 35.1533454, 128.1022747, 15.0], [2, 41, 35.1533898, 128.1022828, 15.0], [2, 42, 35.153434, 128.1022915, 15.0], [2, 43, 35.1534926, 128.1023035, 15.0], [2, 44, 35.1535368, 128.102314, 15.0], [2, 45, 35.1535815, 128.1023233, 15.0], [2, 46, 35.1536261, 128.1023334, 15.0], [2, 47, 35.15367, 128.1023431, 15.0], [2, 48, 35.1537144, 128.1023515, 15.0], [2, 49, 35.1537583, 128.1023609, 15.0], [2, 50, 35.1538027, 128.1023696, 15.0], [2, 51, 35.1538468, 128.1023787, 15.0], [2, 52, 35.1538912, 128.1023877, 15.0], [2, 53, 35.1539354, 128.1023958, 15.0], [2, 54, 35.15398, 128.1024035, 15.0], [2, 55, 35.1540247, 128.1024129, 15.0], [2, 56, 35.1540688, 128.1024226, 15.0], [2, 57, 35.1541135, 128.1024316, 15.0], [2, 58, 35.1541577, 128.1024409, 15.0], [2, 59, 35.1542017, 128.1024499, 15.0], [2, 60, 35.1542463, 128.1024596, 15.0], [2, 61, 35.1542909, 128.1024685, 15.0], [2, 62, 35.1543353, 128.1024776, 15.0], [2, 63, 35.1543796, 128.1024861, 15.0], [2, 64, 35.1544236, 128.1024958, 15.0], [2, 65, 35.154468, 
128.1025049, 15.0], [2, 66, 35.1545124, 128.1025143, 15.0], [2, 67, 35.1545572, 128.1025233, 15.0], [2, 68, 35.1546011, 128.1025331, 15.0], [2, 69, 35.1546453, 128.1025419, 15.0], [2, 70, 35.1546893, 128.1025507, 15.0], [2, 71, 35.154734, 128.1025594, 15.0], [2, 72, 35.1547781, 128.1025684, 15.0], [2, 73, 35.1548221, 128.1025776, 15.0], [2, 74, 35.1548663, 128.1025862, 15.0], [2, 75, 35.1549108, 128.1025961, 15.0], [2, 76, 35.1549556, 128.102605, 15.0], [2, 77, 35.1549996, 128.102615, 15.0], [2, 78, 35.155044, 128.1026237, 15.0]],
3: [[3, 79, 35.1550369, 128.1026779, 15.0], [3, 80, 35.1550296, 128.102732, 15.0], [3, 81, 35.1550217, 128.1027867, 15.0], [3, 82, 35.1550144, 128.1028408, 15.0], [3, 83, 35.1550073, 128.1028952, 15.0], [3, 84, 35.1549995, 128.1029496, 15.0], [3, 85, 35.1549918, 128.1030036, 15.0], [3, 86, 35.1549847, 128.1030576, 15.0], [3, 87, 35.154978, 128.1031122, 15.0], [3, 88, 35.1549692, 128.1031659, 15.0], [3, 89, 35.1549624, 128.1032199, 15.0], [3, 90, 35.1549556, 128.1032738, 15.0], [3, 91, 35.1549486, 128.1033285, 15.0], [3, 92, 35.154941, 128.1033826, 15.0]]}
'''

# 100m straight course
waypoints = {1: [[1, 1, 35.1535098, 128.1002861, 15.0], [1, 2, 35.1535017, 128.1003405, 15.0], [1, 3, 35.153494, 128.1003952, 15.0], [1, 4, 
35.1534847, 128.1004491, 15.0], [1, 5, 35.153477, 128.1005038, 15.0], [1, 6, 35.1534693, 128.1005578, 15.0], [1, 7, 35.1534608, 
128.1006118, 15.0], [1, 8, 35.1534523, 128.1006654, 15.0], [1, 9, 35.1534447, 128.1007197, 15.0], [1, 10, 35.1534351, 128.100773, 15.0], [1, 11, 35.1534271, 128.1008267, 15.0], [1, 12, 35.1534208, 128.1008813, 15.0], [1, 13, 35.1534126, 128.100935, 15.0], [1, 14, 35.1534038, 128.1009893, 15.0], [1, 15, 35.1533948, 128.1010436, 15.0], [1, 16, 35.1533868, 128.1010972, 15.0], [1, 17, 35.1533789, 128.1011509, 15.0], [1, 18, 35.1533701, 128.1012052, 15.0], [1, 19, 35.1533613, 128.1012592, 15.0], [1, 20, 35.1533517, 128.1013132, 15.0], [1, 21, 35.1533446, 128.1013675, 15.0], [1, 22, 35.1533369, 128.1014218, 15.0], [1, 23, 35.1533298, 128.1014758, 15.0], [1, 24, 35.153321, 128.1015294, 15.0], [1, 25, 35.1533134, 128.1015834, 15.0], [1, 26, 35.1533043, 128.1016374, 15.0], [1, 27, 35.1532961, 128.101691, 15.0], [1, 28, 35.1532879, 128.1017447, 15.0], [1, 29, 35.1532796, 128.1017993, 15.0], [1, 
30, 35.15327, 128.1018529, 15.0], [1, 31, 35.1532618, 128.1019066, 15.0], [1, 32, 35.1532544, 128.1019609, 15.0], [1, 33, 35.1532465, 128.1020152, 15.0], [1, 34, 35.1532388, 128.1020695, 15.0], [1, 35, 35.1532308, 128.1021232, 15.0], [1, 36, 35.1532218, 128.1021772, 15.0], [1, 37, 35.1532119, 128.1022452, 15.0]]}


def test():
    start_time = time.time()
    arm_and_takeoff_to_pixhawk(15.0)

    add_waypoints_to_pixhawk(waypoints[1])
    
    set_airspeed_to_pixhawk(40)

    while True:
        elapsed_time = time.time() - start_time
        print("time : ", elapsed_time)
        print_current_gps()
        time.sleep(2)
        if elapsed_time > 90:
            break
    vehicle.close()

test()