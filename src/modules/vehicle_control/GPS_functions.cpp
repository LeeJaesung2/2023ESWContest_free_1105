#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <corecrt_math_defines.h>

#include "GPS_functions.h"

#define LATITUDE 1
#define LONGITUDE 2

std::string rawGps2degGps(int type, std::string token) {
    int degrees;
    double minutes;

    if (type == LATITUDE) { // latitude 
        degrees = stoi(token.substr(0, 2));
        minutes = stod(token.substr(2));
    }
    else if (type == LONGITUDE) { // longitude
        degrees = stoi(token.substr(0, 3));
        minutes = stod(token.substr(3));
    }

    double deg_minutes = minutes / 60;
    std::string str_minutes = std::to_string(deg_minutes);
    str_minutes.erase(std::remove(str_minutes.begin(), str_minutes.end(), '.'), str_minutes.end()); // ���� ����

    std::string what3words = std::to_string(degrees).append(".").append(str_minutes);

    return what3words;
}

GPSData extract_gps_data(const std::string& gps_str) {
    GPSData gps_data;

    // ���ڿ��� ','�� �����Ͽ� ���ڿ� ��Ʈ���� ����
    std::stringstream ss(gps_str);
    std::string token;

    // GPGGA �±� ����
    getline(ss, token, ',');

    // �ð� ���� ����
    getline(ss, token, ',');
    gps_data.time = std::stod(token);

    // ���� ���� ����
    getline(ss, token, ',');
    std::string latitude = rawGps2degGps(LATITUDE, token);
    gps_data.latitude = std::stod(latitude);

    // ����/���� ���� ����
    getline(ss, token, ',');

    // �浵 ���� ����
    getline(ss, token, ',');
    std::string longitude = rawGps2degGps(LONGITUDE, token);
    gps_data.longitude = std::stod(longitude);

    // ����/���� ���� ����
    getline(ss, token, ',');

    return gps_data;
}

double calc_distance(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371e3; // ���� ������
    double phi1 = lat1 * M_PI / 180; // ���� 1
    double phi2 = lat2 * M_PI / 180; // ���� 2
    double delta_phi = (lat2 - lat1) * M_PI / 180; // ���� ����
    double delta_lambda = (lon2 - lon1) * M_PI / 180; // �浵 ����

    double a = sin(delta_phi / 2) * sin(delta_phi / 2) +
        cos(phi1) * cos(phi2) *
        sin(delta_lambda / 2) * sin(delta_lambda / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return R * c; // in meters
}