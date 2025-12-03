#include "sensor_fusion.h"
using namespace std ; 

#include <iostream>
#include <vector> 

SensorSnapshot SensorFusion :: combine(const vector<RawSensorData> &batch)
{
    SensorSnapshot out{} ; 

    out.timestamp = chrono :: steady_clock :: now(); 

    RawSensorData lidar, cam, gps; 

    for (auto &d : batch)
    {
        if (d.type == "lidar") lidar = d ; 
        if (d.type == "Camera") cam = d;  
        if (d.type == "gps") gps = d; 
    }

    out.distance = fuse_distance(lidar, cam); 
    out.lastitude = gps.value1; 
    out.longitude = gps.value2; 

    out.confidence = (lidar.confidence + cam.confidence + gps.confidence) / 3.0;
    
    return out; 
}

double SensorFusion :: fuse_distance(const RawSensorData &lidar, const RawSensorData &cam)
{
    if (lidar.confidence > cam.confidence) return lidar.value1; 
    return cam.value1; 
}