#include <iostream>
#include <vector>

using namespace std;

#define WAYPOINT_DISTANCE 5 // unit: Meter

double getVelocity() {
    return 12.0;
}

vector<int> getWaypoints() { // �ʱ� road_id�� 1������ �����ϱ�
    vector<int> waypoints;

    //map���� waypoint �ҷ�����
    //�켱 20���� waypoint �ִٰ� ����
    for (int i = 1; i <= 20; i++) {
        waypoints.push_back(i);
    }
    return waypoints;
}

int main() {
    double vehicle_velocity = 0.0, remain_distance = 0.0;
    int total_passed_waypoint = 0, passed_num_waypoint = 0, sum_remain_distance = 0;

    vector<int> waypoints = getWaypoints(); // waypoint �������� �Լ� ���� �ʿ�

    int remain_waypoints = waypoints.size();

    while (remain_waypoints > 0) {
        vehicle_velocity = getVelocity();// ���� : m/s => �ӵ� �������� �Լ� ���� �ʿ�

        passed_num_waypoint = static_cast<int>(vehicle_velocity / WAYPOINT_DISTANCE);  // ��������Ʈ�� ������ ����

        // ��������Ʈ�� ��Ȯ�� ������ �������� ������
        remain_distance = vehicle_velocity - (passed_num_waypoint * WAYPOINT_DISTANCE);  // ���� �Ÿ�
        if (remain_distance > 0) {
            sum_remain_distance += remain_distance;
            cout << "sum_remain_distance: " << sum_remain_distance << "\n";
            if (sum_remain_distance >= WAYPOINT_DISTANCE) {
                passed_num_waypoint++;
                sum_remain_distance -= WAYPOINT_DISTANCE;
            }
        }

        total_passed_waypoint += passed_num_waypoint;
        if (total_passed_waypoint > waypoints.size()) {
            total_passed_waypoint = waypoints.size();
        }
        cout << "���� " << total_passed_waypoint << "���� ��������Ʈ�� �������ϴ�.\n";
 

        remain_waypoints -= passed_num_waypoint;
        cout << "remain_waypoints: " << remain_waypoints << "\n\n";

        if (remain_waypoints > 4) {
            // GPS ������ ������ if�� 
            // GPS_main.cpp�� road_id ã�ƾ���
        }

        vehicle_velocity += 4;
    }

    return 0;
}