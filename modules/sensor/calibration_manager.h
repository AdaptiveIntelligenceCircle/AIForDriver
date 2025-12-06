#pragma once 
#include <iostream> 

#include <unordered_map>
#include <string> 

#include <chrono>
using namespace std; 

struct CalibrationData
{
    double offset = 0.0; 
    double scale = 1.0; 
    double drift = 0.0; 

    chrono :: steady_clock :: time_point last_update; 
}; 

class CalibrationManager
{
    public: 
    CalibrationManager() = default;
    ~CalibrationManager() = default; 
    
    // load calibration for a sensor (from file, Db, or preset)..
    void load(const string &sensor, const CalibrationData &data); 

    // auto calibrate based on raw input .. 
    void update_runtime(const string &sensor, double raw_value);

    // get calibrated value from raw input. 
    double apply(const string &sensor, double raw_value); 

    // check stalemates of calibration
    bool is_stale(const string &sensor, double max_sec = 30.0) const; 

    private: 
    unordered_map<string, CalibrationData> table ; 
}; 