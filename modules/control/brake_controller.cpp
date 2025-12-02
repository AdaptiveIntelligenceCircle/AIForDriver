#include "brake_controller.h"
#include <algorithm>
using namespace std; 

double BrakeController :: compute(double force, double dt)
{
    double new_val = last_brake + force *dt; 
    new_val = clamp(new_val, 0.0, 1.0); 

    last_brake = new_val; 
    return new_val;
}