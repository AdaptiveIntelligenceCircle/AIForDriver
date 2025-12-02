#pragma once 

#include "control_state.h"

class ControlPolicy
{
    public: 
    void apply_policy(ControlState &s); 

    // return desired accelarations (- decel + accel).. 
    double compute_accelaration(const ControlState &s); 
}; 