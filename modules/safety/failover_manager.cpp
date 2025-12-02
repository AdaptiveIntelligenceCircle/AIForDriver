#include "failover_manager.h"
#include "safety_log.h"
using namespace std; 
#include <thread> 

FailoverManager :: FailoverManager(){}
FailoverManager :: ~FailoverManager(){}

void FailoverManager :: engage_safe_mode(const string &reason)
{
    SafetyLog :: instance().warn("Engaging safe mode:" + reason); 
    // example : notify runtime, lower speed cap, disable adaption.. 
}

void FailoverManager :: throttle_noncritical(const string &channel, int durations_ms)
{
    SafetyLog :: instance().info("Throttle Channel" + channel + "for" + to_string(durations_ms) + "ms");
    thread([channel, durations_ms]()
    {
        this_thread :: sleep_for(chrono :: milliseconds(durations_ms)); 
        SafetyLog :: instance().info("Throttle expired:" + channel); 
    }).detach(); 
}

void FailoverManager :: set_rollback_hook(function<void(int snapshot_id)>cb)
{
    rollback_hook = move(cb); 
}

void FailoverManager :: schedule_rollback(int snapshot_id)
{
    if (rollback_hook){
        thread([this, snapshot_id]()
        {
            try {rollback_hook(snapshot_id); }
            catch(...){}
        }).detach(); 
    }
    else {
        SafetyLog :: instance().error("No Rollback hook registered"); 
    }
}