#pragma once 

class BrakeController
{
    public:
    double compute(double braking_force, double dt); 

    private: 
    double last_brake = 0.0 ; 
}; 