#pragma once 

class SteeringController
{
    public:
    SteeringController() = default; 

    ~SteeringController() = default; 

    double compute(double target_angle, double dt); 

    private: 
    double last_output = 0.0; 
    const double max_rate = 40.0; 
};