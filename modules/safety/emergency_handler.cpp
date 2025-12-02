#include "emergency_handler.h"

using namespace std; 
#include "safety_log.h"

#include <thread> 
#include <iostream>
#include <functional>

EmergencyHandler :: EmergencyHandler(){}
EmergencyHandler :: ~EmergencyHandler(){}

void EmergencyHandler :: set_emergency_hook(function<void(const string &reason)> h)
{
    lock_guard<mutex> lock(mtx); 
    emergency_hook = move(h); 
}

void EmergencyHandler :: trigger (const string &reason)
{
    
}