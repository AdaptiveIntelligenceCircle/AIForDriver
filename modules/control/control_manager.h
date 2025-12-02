#pragma once 
#include <mutex> 

#include "control_state.h"
#include "steering_controller.h"

#include "throttle_controller.h"
#include "brake_controller.h"

#include "motion_controller.h"
#include "control_policy.h"

#include "safety_controller.h"
using namespace std; 

class ControlManager
{
    public: 
    ControlManager(); 
    ~ControlManager(); 

    // main entry: runtime will call this each cycle.. 
    void update(double dt); 

    // input from sensors/runtime.. 
    void set_target_speed(double s);  
    void set_navigation_angle(double angle); 

    // output (to actuators).. 
    ControlState get_state() const; 

    // safety input.. 
    void trigger_emergency(); 

    private: 
    mutable mutex mtx; 

    ControlState state; 
    SteeringController steering; 
    ThrottleController throttle; 
    BrakeController brake; 
    MotionController motion; 
    ControlPolicy policy; 
    SafetyController safety; 

    double nav_angle = 0.0; 

}; 