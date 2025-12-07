#include "calibration_manager.h"
#include <iostream>

#include <cmath> 
#include "safety/safety_log.h"

using namespace std ; 

void CalibrationManager :: load(const string &sensor, const CalibrationData &data)
{
    table[sensor] = data; 
    SafetyLog :: instance().info("Calibration Loaded for :" + sensor); 
}

void CalibrationManager :: update_runtime(const string &sensor, double raw_value)
{
    auto it = table.find(sensor); 

    if (it == table.end()) return; 

    CalibrationData &d = it -> second; 

    // simple drift estimations : if value stable but offset shifts slowly.. 
    double drift_est = raw_value - (raw_value - d.offset) * d.scale; 

    // low pass filter for drift..
    d.drift = 0.9  * d.drift + 0.1 * drift_est; 

    d.last_update = chrono :: steady_clock :: now(); 
}

double CalibrationManager :: apply(const string &sensor, double raw_value)
{
    auto it = table.find(sensor); 
    if (it == table.end()) return raw_value; 

    CalibrationData &d  = it -> second;

    double calibrated  = (raw_value - d.offset - d.drift) * d.scale; 
    return calibrated; 
}

bool CalibrationManager :: is_stale(const string &sensor, double max_sec) const 
{
    auto it = table.find(sensor); 
}