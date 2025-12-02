#pragma once 
#include <functional>
#include <mutex> 

#include <atomic>
#include <chrono> 

#include <map> 
#include <unordered_map>

using namespace std; 

struct SensorSnapshot
{
    double distance_to_object = 999.0; 
    double attention = 1.0; 
    double faitgue = 0.0; 

    chrono :: steady_clock :: time_point last_update; 
    // other fields as needed.. 
}; 

struct DriverSnapshot
{
    double attention = 1.0; 
    double fatigue = 0.0; 
    bool engaged  = true; 
}; 

class SafetyMonitor
{
    public: 
    using ThresholdCallback = function<void(const string &type, double value)>;
    using IntrusionCallback = function<void (const string &reason, double serverity)>;
    
    SafetyMonitor(); 
    ~SafetyMonitor(); 

    void start(); 
    void stop();
    
    // Feed inputs.. 
    void feed_sensor(const SensorSnapshot &s); 
    void feed_driver(const DriverSnapshot &d); 

    //  heartbeat / intrusion reports.. 
    void report_intrusion(const string &reason, double severity); 

    // Register callbacks... 
    void set_threshold_callback(ThresholdCallback cb); 
    void set_intrusion_callback(ThresholdCallback cb); 

    private: 
    mutex mtx; 
    atomic<bool> running; 

    ThresholdCallback threshold_cb; 
    IntrusionCallback intrusion_cb; 

    // thresholds (configurable).. 
    double collision_distance_threshold = 5.0; // meters..
    double fatigue_threshold = 0.8; 
    double attention_threshold = 0.2; 
    double sensor_timeout_seconds = 1.5;  

    // last sensor timestamps.. 
    unordered_map<string, chrono :: steady_clock :: time_point > last_seen; 
}; 