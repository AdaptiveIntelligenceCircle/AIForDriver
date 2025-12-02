#pragma once 
#include "safety_monitor.h"

#include <iostream>

class Validator
{
    public: 
    Validator(); 
    ~Validator(); 

    bool validate_sensor(const SensorSnapshot &s); 
    bool validate_driver(const DriverSnapshot &d); 
}; 