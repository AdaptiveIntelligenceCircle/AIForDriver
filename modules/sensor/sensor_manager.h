#pragma once 
#include <vector> 

#include <memory> 
#include <thread> 

#include <atomic> 
#include <functional>

#include "sensor_base.h"
#include "sensor_fusion.h"

#include "sensor_health.h"
using namespace std; 

struct SensorSnapshot; 

class SensorManager
{
    public: 
    using SensorSnapshot = function<void(const SensorSnapshot &)>;

    SensorManager(); 
    ~SensorManager(); 

    void register_sensor(unique_ptr<SensorBase> s); 
    void set_callback(SensorSnapshot cb); 

    void start(); 
    void stop(); 

    private :
    void loop();  

    vector<unique_ptr <SensorBase>> sensors; 
    unique_ptr<SensorFusion> fusion; 
    unique_ptr<SensorHealth> health; 

    // SnapshotCallback snapshot_cb; 

    thread polling_thread; 
    atomic<bool> running; 
    int polling_hz = 30; 
};  