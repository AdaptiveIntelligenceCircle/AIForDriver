// safety_policy.cpp
#include "safety_policy.h"
#include <iostream> 
SafetyPolicy::SafetyPolicy() : mode(Mode::HUMAN_PRIORITY) 
{

}

SafetyPolicy::~SafetyPolicy() 
{
    std :: cout <<  std:: endl; 
}

void SafetyPolicy::set_mode(Mode m) 
{ 
    mode = m; 
}

SafetyPolicy::Mode SafetyPolicy::current_mode() const 
{ 
    return mode; 
}
