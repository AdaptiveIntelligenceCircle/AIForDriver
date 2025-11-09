#pragma once 
#include <string> 

#include <functional>
#include <chrono>

#include <mutex> 
#include <vector> 

#include <atomic> 
using namespace std;

#include <thread>

// Scheduler : dang ky cac nhiem vu chu ky(periodic)
// tasks chay bang cach callback -> thuong se post vao Executor..
// don gian , thread-safe basic scheduler

class Scheduler
{
    public: 
    using Clock = chrono :: steady_clock; 
    using TimePoint = Clock :: time_point; 
    using Duration = Clock :: duration; 
    using Taskfn  = function<void()>; 

    Scheduler(); 
    ~Scheduler(); 

    // Non - copyable.. 
    Scheduler(const Scheduler&) = delete; 
    Scheduler &operator =(const Scheduler&) = delete; 

    // Schedule periodic task, return task id..// 
    int schedule_periodic(Taskfn fn, Duration period, Duration initial_delay = Duration :: zero());

    // Schedule one-shot after delay.. / 
    int schedule_once(Taskfn fn, Duration delay); 

    // Cancel task by id 
    void cancel(int task_id); 

    // start / loop scheduler loop
    void start(); 
    void stop();

    private: 
    struct Task
    {
        /* data */
        int id; 
        Taskfn fn; 
        TimePoint next_run; 
        Duration period; 
        bool cancelled = false; 
    };

    void run_loop(); 

    mutex mtx; 
    vector<Task> tasks; 
    atomic <bool> running; 
    thread loop_thread; 
    int next_id; 
    
}; 