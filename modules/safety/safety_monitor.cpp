#include "safety_monitor.h"
#include "safety_log.h"

#include <thread> 
using namespace std;

#include <iostream>

SafetyMonitor :: SafetyMonitor() : running(false){}
SafetyMonitor :: ~SafetyMonitor() {
    stop(); 
}

void SafetyMonitor :: stop()
{
    running = false;
    // SafetyLog  :: instance().info("SafetyMonitor stopped"); 
}