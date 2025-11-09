#pragma once 
#include "executor.h"

#include "scheduler.h"
#include "bridge_lua.h"

#include <atomic> 
#include <memory> 

using namespace std; 

class EventLoop
{
    public: 
    EventLoop(); 
    ~EventLoop();

    // Non - copyable
    EventLoop(const EventLoop&) = delete; 
    EventLoop &operator = (const EventLoop&) = delete; 

    // Intialize components(optional params)
    void init(size_t worker_count = 0); 

    // start / stop loop.. 
    void start(); 
    void stop(); 

    // Access components.
    Executor &executor(); 
    Scheduler &scheduler(); 
    BridgeLua &lua_bridge(); 

    private: 
    unique_ptr<Executor> exec; 
    unique_ptr<Scheduler> sched; 
    unique_ptr<BridgeLua> bridge; 
    atomic<bool> running; 
}; 