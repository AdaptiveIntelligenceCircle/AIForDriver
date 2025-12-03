#pragma once 
#include <vector> 

#include "sensor_base.h"
using namespace std; 

struct SensorSnapshot
{
    double distance = 999; 
    double velocity = 0; 
    
    double lastitude = 0; 
    double longitude = 0; 

    double confidence = 1.0; 
    chrono :: steady_clock :: time_point timestamp; 
}; 

class SensorFusion
{
    public: 
    SensorFusion() = default; 

    SensorSnapshot combine(const vector<RawSensorData> &batch);
    
    private: 
    double fuse_distance(const RawSensorData &lidar, const RawSensorData &camera); 
}; 