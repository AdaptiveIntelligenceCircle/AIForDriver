#pragma once 
#include <string>

#include <algorithm>
#include <random>

#include "sensor_base.h"
#include <mutex> 
using namespace std; 

class SensorIMU : public SensorBase
{
    public: 
    SensorIMU(); 
    ~SensorIMU() override  = default;
    
    bool start() override ; 
    void stop() override; 

    RawSensorData read() override; 
    string name() const override 
    {
        return "imu"; 
    }

    // configure noise and bias.. 
    void set_accel_noise_sd(double s); 
    void set_gyro_noise_sd(double s); 
    void set_bias_drift(double accel_bias_drift_per_sec, double gyro_bias_drift_per_sec); 

    private: 
    mutex mtx; 
    bool active; 

    double accel_bias; 
    double gyro_bias;
    double accel_bias_drift_per_sec; 
    double gyro_bias_drift_per_sec; 

    double accel_noise_sd; 
    double gyro_noise_sd; 

    default_random_engine re; 
    normal_distribution<double> accel_noise; 
    normal_distribution<double> gyro_noise ; 

    // intergration state.. 
    double estimated_velocity; // ms along axis.. 
    chrono :: steady_clock :: time_point last_ts; 
}; 