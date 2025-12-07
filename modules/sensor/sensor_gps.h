#pragma once 
#include <string> 

#include "sensor_base.h"
#include <mutex> 

#include <random> 
using namespace std; 

class SensorGPS : public SensorBase
{
    public: 
    SensorGPS() = default; 
    ~SensorGPS() override = default; 

    bool start() override; 
    void stop() override; 

    RawSensorData read() override; 
    string name() const override {
        return "gps"; 
    }; 

    // configure.. 
    void set_base(double lat, double lon); 
    void set_drift_rate_m_per_sec(double meters_per_sec); // simulate slow drift; 
    void set_jump_probability(double p); 

    private: 
    mutex mtx; 

    // base position (deg)... 
    double base_lat; 
    double base_lon; 

    // internal state..
    bool active; 
    double drift_lat; 

    double drift_rate_mps;
    double jump_prob;

    double alpha; 
    double drift_lon; 

    // random engine.. 
    default_random_engine re; 
    uniform_real_distribution<double> uni01; 
    normal_distribution<double> noise_dist; 

    // helper.. 
    static double meters_to_deg_lat(double m);
    static double meters_to_deg_lon(double m, double lat); 
}; 