#include "sensor_health.h"
using namespace std; 

#include "safety_log.h"
#include <chrono>
#include <cmath> 

bool SensorHealth :: detect_stale(const RawSensorData &d)
{
    auto now = chrono :: steady_clock :: now(); 
    auto diff = chrono :: duration_cast<chrono:: milliseconds> (now - d.timestamp).count(); 
    return diff > 300; // stale > 300 ms// 
}

bool SensorHealth :: detect_spike(const RawSensorData &d)
{
    return abs(d.value1)  > 10000; 
}

void SensorHealth :: evaluate(const vector<RawSensorData> &batch)
{
    for (auto &d : batch)
    {
        if (detect_stale(d))
        {
            SafetyLog:: instance().warn("Sensor stale:" + d.type); 
        }
        if (detect_spike(d))
        {
            SafetyLog :: instance().warn("Sensor spike:" + d.type); 
        }
    }
}