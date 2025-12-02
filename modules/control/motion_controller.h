#pragma once 
#include "control_state.h"

class MotionController
{
    public:
    void compute(ControlState &state, double nav_angle, double dt); 
}; 