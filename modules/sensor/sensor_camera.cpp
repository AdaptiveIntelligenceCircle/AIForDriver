#include "sensor_camera.h"
using namespace std; 

#include <iostream>
#include <string> 

bool SensorCamera :: start()
{
    active = true ; 
    dist = uniform_real_distribution<double>(3.0, 12.0); 
    return true; 
}

void SensorCamera :: stop()
{
    active = false; 
}

RawSensorData SensorCamera :: read()
{
    RawSensorData d; 
    d.type = "Camera"; 
    d.timestamp = chrono :: steady_clock :: now();  

    if (!active){
        d.value1 = 999; 
        d.confidence = 0.0; 
        return d; 
    }
    // simulate random dropout ...
    uniform_real_distribution<double> prob(0.0, 1.0); 
    if (prob (re) <  dropout_prob)
    {
        d.value1 = 999; 
        d.confidence =  0.1; 
        return d; 
    }

    // main simluated detection.. 
    d.value1 = dist(re); 
    d.confidence= 0.85; 

    return d; 
}