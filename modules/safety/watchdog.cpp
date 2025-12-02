#include "watchdog.h"
using namespace std; 

#include "safety_log.h"
#include <iostream>

Watchdog :: Watchdog() : running(false){}
Watchdog :: ~Watchdog(){stop(); }

void Watchdog :: start()
{
    running = true; 
    loop_thread = thread([this]()
    {
        loop_fn(); 
    }); 

    SafetyLog :: instance().info("Watchdog started"); 
}

void Watchdog :: stop()
{
    running = false; 
    if (loop_thread.joinable())
    loop_thread.join(); 
    SafetyLog :: instance().info("Watchdog stopped"); 
}

void Watchdog :: beat(const string &source)
{
    lock_guard<mutex> lock(mtx); 
    last_beat[source] = chrono :: steady_clock :: now(); 
}

void Watchdog :: set_timeout_callback(function<void(const string &)>cb)
{
    timeout_cb = move(cb); 
}

void Watchdog :: loop_fn()
{
    while(running)
    {
        this_thread :: sleep_for(poll_interval); 
        auto now = chrono :: steady_clock :: now(); 
        vector<string> timed_out; 
        {
            lock_guard<mutex> lock(mtx); 
            for (auto &p : last_beat){
                if (now - p.second > timeout_threshild)
                {
                    timed_out.push_back(p.first); 
                }
            }
        }
        for (auto &s : timed_out)
        {
            SafetyLog :: instance().warn("Watchdog timeout:" + s); 
            if (timeout_cb) timeout_cb(s); 
            // mark beat to now to avoid repeated flood.. 
            beat(s);  
        }
    }
}