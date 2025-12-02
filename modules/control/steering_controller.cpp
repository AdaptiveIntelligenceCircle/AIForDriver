#include "steering_controller.h"
using namespace std; 

#include <iostream> 
#include <algorithm>

double SteeringController :: compute(double target_angle, double dt)
{
    double max_delta = max_rate *dt; 
    double delta = target_angle - last_output; 

    if (abs(delta) > max_delta)
    delta = (delta > 0 ? max_delta : -max_delta); 

    last_output += delta; 
    return last_output; 
}