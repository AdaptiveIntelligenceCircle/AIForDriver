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