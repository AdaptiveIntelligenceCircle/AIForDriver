#pragma once 

class ThrottleController 
{
    public:
    double compute(double desired_acc, double dt) ; 

    private: 
    double last_throttle = 0.0; 
    const double max_step = 0.2; // smoothing.. 
}; 