#include "motion_controller.h"
#include <cmath> 

using namespace std; 

void MotionController :: compute(ControlState &state, double nav_angle, double dt)
{
    // update speed.. 
    state.current_speed += (state.throttle - state.brake_force) * 5 *dt;

    if (state.current_speed < 0)
    state.current_speed = 0; 
}