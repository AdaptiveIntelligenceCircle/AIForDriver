#include "control_policy.h"
using namespace std; 

#include <algorithm>
#include <cmath> 

void ControlPolicy :: apply_policy(ControlState &s)
{
    // clamp target speed. 
    if (s.target_speed < 0) s.target_speed = 0; 
}

double ControlPolicy :: compute_accelaration(const ControlState &s)
{
    double error = s.target_speed - s.current_speed; 

    // simple P controller .. 
    double acc = error *0.5; 

    return clamp(acc, -1.0, 1.0); 
}