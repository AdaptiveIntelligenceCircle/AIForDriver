#include "safety_manager.h"
#include "safety_monitor.h"

#include "hazard_assessor.h"
#include "emergency_handler.h"

#include "emergency_handler.h"
#include "watchdog.h"

#include "failover_manager.h"
#include "validator.h"

#include "safety_log.h"
#include <iostream>

using namespace std;

SafetyManager::SafetyManager()
    : monitor(make_unique<SafetyMonitor>()),
      assessor(make_unique<HarzardAssessor>()),
      handler(make_unique<EmergencyHandler>()),
      watchdog(make_unique<WatchDog>()),
      failover(make_unique<FailoverManager>()),
      policy(make_unique<SafetyPolicy>()),
      validator(make_unique<Validator>()),
      running(false)
{
    // wire up monitor -> asessor -> handler.. 
    
}
