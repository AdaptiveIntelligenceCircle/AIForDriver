#pragma once 

struct ControlState{
    double target_speed = 0.0; 
    double current_speed = 0.0; 

    double steering_angle = 0.0; 
    double brake_force = 0.0; 
    double throttle = 0.0; 

    bool emergency_override = false; 
};