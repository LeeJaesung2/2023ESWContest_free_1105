#include <limits>

#include "GPS_functions.h"
#include "Map.h"
#include "MyIMU.h"

using namespace std;

#define WAYPOINT_DISTANCE 5 // unit: Meter

int main() {
    unordered_map<int, Vertex> graph = creatingMap();

    int road_id = 1;
    float vehicle_velocity = 0.0, remain_distance = 0.0;
    size_t total_passed_waypoint = 0;
    int passed_num_waypoint = 0, sum_remain_distance = 0;

    vector<int> waypoints = getWaypoints(graph, road_id);

    int remain_waypoints = waypoints.size();

    while (remain_waypoints > 0) {
        vehicle_velocity = getVelocity();

        passed_num_waypoint = static_cast<int>(vehicle_velocity / WAYPOINT_DISTANCE);

        remain_distance = vehicle_velocity - (passed_num_waypoint * WAYPOINT_DISTANCE);
        if (remain_distance > 0) {
            sum_remain_distance += remain_distance;
            if (sum_remain_distance >= WAYPOINT_DISTANCE) {
                passed_num_waypoint++;
                sum_remain_distance -= WAYPOINT_DISTANCE;
            }
        }

        total_passed_waypoint += passed_num_waypoint;
        if (total_passed_waypoint > waypoints.size()) {
            total_passed_waypoint = waypoints.size();
        }
        cout << "Currently passed " << total_passed_waypoint << " waypoints.\n";

        remain_waypoints -= passed_num_waypoint;
        cout << "remain_waypoints: " << remain_waypoints << "\n\n";

        // �ش� road_id�� ���� ��������Ʈ�� 1�� �̸��̸�
        if (remain_waypoints < 1) {
            // Condition to wake up GPS thread
            // Need to find road_id in GPS_main.cpp

            // ���� GPS���� �о��
            string gps = "$GPGGA,114455.532,3591.9538,N,12861.3522,E,1,03,50.0,0.0,M,19.6,M,0.0,0000*4F";

            // ���� GPS���� gps_function�� �̿��ؼ� ����, �浵�� �����Ѵ�.
            float current_latitude = extract_gps_data(gps).latitude;
            float current_longitude = extract_gps_data(gps).longitude;

            // ���� road_id�� ����Ǿ� �ִ� ��� ���� road_id�� ù waypoint�� ���� �浵�� ���´�.
            vector<int> connected_roads = graph[road_id].connectedRoads;
            float min_distance = numeric_limits<float>::max(); // �ּڰ� �ʱ�ȭ
            int nearest_road_id = -1;
            vector<float> first_waypoint_coordinates;

            for (int connected_road : connected_roads) {
                vector<int> connected_waypoints = getWaypoints(graph, connected_road);
                if (!connected_waypoints.empty()) {
                    int first_waypoint_id = connected_waypoints[0];

                    cout << "first_waypoint_id: " << first_waypoint_id;

                    float waypoint_latitude = graph[connected_road].latitude[0];
                    float waypoint_longitude = graph[connected_road].longitude[0];

                    // ������ ��������Ʈ�� ��� ���� �浵 ����
                    if (first_waypoint_id == 1) {
                        first_waypoint_coordinates = {waypoint_latitude, waypoint_longitude};
                    }

                    // �� gps ���� �Ÿ��� �����Ѵ�.
                    float distance = calc_distance(current_latitude, current_longitude, waypoint_latitude, waypoint_longitude);
                    if (distance < min_distance) {
                        min_distance = distance;
                        nearest_road_id = connected_road;
                    }

                    cout << "     distance: " << distance << "\n";
                }
            }

            // �Ÿ��� ��ȿ�� ������ Ȯ���ϰ�, 3m �̳��� ���� �ش� road_id�� �����Ѵ�
            if (nearest_road_id != -1 && min_distance <= 3) {
                road_id = nearest_road_id;
                waypoints = getWaypoints(graph, road_id);
                remain_waypoints = waypoints.size();
                cout << "Switched to road ID: " << road_id << endl;
                cout << "remain_waypoints: " << remain_waypoints << endl << endl;
            }
        }
    }

    return 0;
}
