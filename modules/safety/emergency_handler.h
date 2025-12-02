#pragma once 
#include <iostream>

#include <string>
#include <vector> 

#include <mutex>
#include <string> 

using namespace std; 

class EmergencyHandler
{
    public: 
    EmergencyHandler(); 
    ~EmergencyHandler(); 

    // register hook to call for low level emergency.. 
    void set_emergency_hook(function<void(const string &reason)> h);
    
    // perform emergency sequence 
    void trigger(const string &reason); 

    private: 
    function<void(const string&) > emergency_hook; 
    mutex mtx; 
}; 