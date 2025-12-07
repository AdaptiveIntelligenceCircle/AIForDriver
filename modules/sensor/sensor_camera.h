#pragma once
#include "sensor_base.h"

#include <string> 
#include <random> 

using namespace std; 

class SensorCamera : public SensorBase
{
    public: 
    SensorCamera() = default; 
    ~SensorCamera() = default; 

    bool start() override; 
    void stop() override; 

    RawSensorData read() override; 

    string name() const override {
        return "camera"; 
    }; 

    private: 

    bool active = false; 
    // Fake detection for now .. 
    default_random_engine re; 
    uniform_real_distribution<double> dist; 

    // probability of losing detection (for testing safety//).. 
    double dropout_prob = 0.05; 
}; 