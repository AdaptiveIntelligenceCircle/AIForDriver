#include "sensor_manager.h"
#include "sensor_utils.h"
#include "safety_log.h"

using namespace std; 

SensorManager :: SensorManager()
: fusion(make_unique<SensorFusion>()),
    health(make_unique<SensorHealth>()),
    running(false)
{

}

SensorManager :: ~SensorManager (){
    stop(); 
}

void SensorManager :: register_sensor(unique_ptr<SensorBase>s)
{
    sensors.push_back(move(s)); 
}

// void SensorManager :: set_callback(SnapshotCallback &cb)
// {
//     snapshot_cb =  move(cb);
// }

void SensorManager :: start()
{
    running = true; 
    for (auto &s : sensors) s -> start(); 

    polling_thread = thread([this](){
        loop(); 
    }); 
}

void SensorManager :: stop()
{
    running = false; 
    if (polling_thread.joinable()){
        polling_thread.join(); 
    }
    for (auto &s : sensors) s -> stop(); 
}

void SensorManager :: loop()
{
    const int interval_ms = 1000 / polling_hz; 

    while (running)
    {
        vector<RawSensorData> batch; 
        batch.reserve(sensors.size()); 

        for (auto &s : sensors){
            batch.push_back(s -> read()); 
        }

        // health check..// 
        health -> evaluate(batch); 

        // fusion.. 
        // SensorSnapshot snapshot = fusion -> combine(batch); 

        // emit to safety/analytics/runtime.. // 
        // if (snapshot_cb) snapshot_cb(snapshot);

        this_thread :: sleep_for(chrono :: milliseconds(interval_ms)); 
    }
}