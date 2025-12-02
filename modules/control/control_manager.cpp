#include "control_manager.h"
using namespace std; 
#include <algorithm>

ControlManager :: ControlManager(){}
ControlManager :: ~ControlManager(){}

void ControlManager :: set_target_speed(double s)
{
    lock_guard<mutex> lock(mtx); 
    state.target_speed = max(0.0, s); 
}

void ControlManager :: set_navigation_angle(double angle)
{
    lock_guard<mutex> lock(mtx); 
    nav_angle  = angle;
}

void ControlManager :: trigger_emergency()
{
    lock_guard<mutex> lock(mtx); 
    state.emergency_override = true; 
}

ControlState ControlManager :: get_state() const 
{
    lock_guard<mutex> lock(mtx); 
    return state; 
}

void ControlManager :: update(double dt)
{
    lock_guard<mutex> lock(mtx); 

    // 1> Safety Override First.. 
    if (state.emergency_override)
    {
        safety.apply_emergency(state); 
        return; 
    }

    // 2> Normal policy .. 
    policy.apply_policy(state); 

    // 3> Motion controller generates trajectory// 
    motion.compute(state, nav_angle,dt); 

    /// 4> STeering 
    state.steering_angle = steering.compute(nav_angle, dt);

    // 5> Throttle and braking angle.. 
    double desired_acc = policy.compute_accelaration(state); 

    if (desired_acc < 0)
    {
        state.brake_force = brake.compute(-desired_acc, dt); 
        state.throttle = 0.0; 
    }
    else{
        state.brake_force = 0.0; 
        state.throttle = throttle.compute(desired_acc, dt); 
    }
}