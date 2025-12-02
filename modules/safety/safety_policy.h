#pragma once 
#include <string> 

using namespace std; 

class SafetyPolicy
{
    public: 
    enum class Mode
    {
        HUMAN_PRIORITY, BALANCED, PERFORMANCE
    };
    
    SafetyPolicy(); 
    ~SafetyPolicy(); 

    void set_mode(Mode m); 
    Mode current_mode() const; 

    private: 
    Mode mode; 
}; 