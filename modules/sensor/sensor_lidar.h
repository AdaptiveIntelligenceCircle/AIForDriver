#pragma once 
#include <iostream>

#include <string> 
using namespace std; 

#include <cmath> 
#include <random>

#include "sensor_base.h"
#include <mutex> 

class SensorLiDar : public SensorBase
{
    public: 
    SensorLiDar();
    ~SensorLiDar() override = default; 

    bool start() override; 
    void stop() override; 

    RawSensorData read() override ; 
    string name() const override {
        return "lidar"; 
    }

    void set_max_range(double r);
    void set_beam_error_sd(double sd); 
    void set_occlusion_probability(double p); 

    private: 
    mutex mtx; 
    bool active; 

    double max_range_m; 
    double beam_error_sd; 
    double occlusion_prob;

    default_random_engine re; 
    normal_distribution<double> beam_noise; 
    uniform_real_distribution<double> uni01;  
}; 