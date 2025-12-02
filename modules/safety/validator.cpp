// validator.cpp
#include "validator.h"

Validator::Validator() 
{

}

Validator::~Validator() 
{

}

bool Validator::validate_sensor(const SensorSnapshot& s) 
{
    // sanity checks: distances non-negative, timestamps reasonable etc.
    if (s.distance_to_object < 0) return false;
    return true;
}

bool Validator::validate_driver(const DriverSnapshot& d) 
{
    if (d.attention < 0.0 or d.attention > 1.0) 
        return false;
    if (d.fatigue < 0.0 or d.fatigue > 1.0) 
        return false;
    return true;
}
