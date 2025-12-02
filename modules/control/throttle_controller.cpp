#include "throttle_controller.h"
#include <algorithm>

#include <iostream>
using namespace std; 

double ThrottleController :: compute(double desired_acc, double dt)
{
    double new_val = last_throttle + desired_acc *dt ; 

    // Clamp and smooth.. 
    new_val = clamp(new_val, last_throttle - max_step, last_throttle); 
    new_val = clamp(new_val, 0.0, 1.0); 

    last_throttle = new_val; 
    return new_val; 
}