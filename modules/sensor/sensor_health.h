#pragma once 
#include <vector> 

#include <string> 
#include "sensor_base.h"

using namespace std; 

class SensorHealth
{
    public:
    void evaluate(const vector<RawSensorData> &batch); 

    private: 
    bool detect_stale(const RawSensorData &d); 
    bool detect_spike(const RawSensorData &d); 
}; 