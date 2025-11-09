#include "event_loop.h"
#include <iostream> 

using namespace std; 

EventLoop :: EventLoop() : running(false){}

EventLoop :: ~EventLoop()
{
    stop(); 
}

void EventLoop :: init(size_t worker_count)
{
    if (!exec) exec = std::make_unique<Executor>(worker_count == 0 ? std::thread::hardware_concurrency() : worker_count);
    if (!sched) sched = std::make_unique<Scheduler>();
    if (!bridge) bridge = std::make_unique<BridgeLua>();
}

void EventLoop :: start()
{
    bool exp = false; 
    if (!running.compare_exchange_strong(exp, true)) return; 

    if (!exec or !sched or !bridge)
    {
        init(); 
    }

    sched -> start(); 

    // Example: schedule a periodic heartbeat.. 
    sched -> schedule_periodic([this]()
    {
        this -> exec -> post([]()
        {

        }); 
    }, chrono :: milliseconds(200), chrono:: milliseconds(50));  
}

void EventLoop :: stop()
{
    bool exp = true; 
    if (running.compare_exchange_strong(exp, false)) {
        if (sched) sched->stop();
        if (exec) exec->shutdown();
        // bridge will be destroyed on unique_ptr destructor
    }
}

Executor& EventLoop::executor() 
{
    if (!exec) init();
    return *exec;
}

Scheduler& EventLoop::scheduler() 
{
    if (!sched) init();
    return *sched;
}

BridgeLua& EventLoop::lua_bridge() 
{
    if (!bridge) init();
    return *bridge;
}