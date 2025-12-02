#pragma once

#include "control_state.h"
using namespace std; 

class SafetyController
{
    public: 
    void apply_emergency(ControlState &state)
    {
        state.throttle = 0.0; 
        state.brake_force = 1.0; 
        state.steering_angle = 0.0; 
    }
};