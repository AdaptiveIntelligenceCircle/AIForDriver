#pragma once 
#include <string> 

#include <chrono> 
#include <iostream>

using namespace std; 

struct RawSensorData
{
    string type; 
    chrono :: steady_clock :: time_point timestamp; 
    double value1 = 0; 
    double value2 = 0; 
    double confidence = 1.0; 
}; 

class SensorBase
{
    public: 
    virtual ~SensorBase() = default; 

    virtual bool start() = 0 ;
    virtual void stop() = 0; 

    // Read raw data from hardware / driver.. 
    virtual RawSensorData read() = 0 ; 

    virtual string name() const = 0; 
}; 