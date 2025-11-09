#include "scheduler.h"
#include <thread> 

#include <iostream>
using namespace std; 

Scheduler ::Scheduler(): running(false), next_id(1){}

Scheduler :: ~Scheduler(){
    stop();
}

int Scheduler :: schedule_periodic(Taskfn fn, Duration period, Duration initial_delay = Duration :: zero()){
    lock_guard<mutex> lock(mtx); 
    Task t ; 
    t.id = next_id ++; 
    t.fn = move(fn); 
    t.period = period; 
    t.next_run = Clock :: now() + initial_delay; 
    tasks.push_back(move(t)); 
    return t.id; 
}

void Scheduler :: cancel(int task_id)
{
    lock_guard <mutex> lock(mtx); 
    for (auto &t : tasks)
    {
        if (t.id == task_id)
        {
            t.cancelled = true; 
            break; 
        }
    }
}

void Scheduler :: start()
{
    bool exp = false; 
    if (!running.compare_exchange_strong(exp, true)) return; 
    loop_thread = thread([this]()
    {
        run_loop(); 
    }); 
}

void Scheduler :: stop()
{
    bool exp = true; 
    if (running.compare_exchange_strong(exp, false)){
        if (loop_thread.joinable())
        loop_thread.join(); 
    }
}

void Scheduler :: run_loop()
{
    using namespace chrono; 
    while (running)
    {
        auto now = Clock :: now(); 
        vector<Task> due; 
        {
            lock_guard<mutex> lock(mtx); 
            for (auto &t : tasks){
                if (t.cancelled ) continue;
                if (t.next_run <= now)
                {
                    due.push_back(t); 
                    if (t.period == Duration :: zero())
                    {
                        t.cancelled = true ; 
                    }
                    else{
                        t.next_run = now + t.period; 
                    }
                }
            }
            // remove cancelled tasks..//
            tasks.erase(remove_if(tasks.begin(), tasks.end(),
                                       [](const Task& t){ return t.cancelled; }),
                        tasks.end());
        }

        for (auto &t: due){
            try 
            {
                t.fn(); 
            }
            catch(const exception &e)
            {
                cout <<"Scheduler - task exception:" << e.what() << endl; 
            }
            catch(...)
            {
                cout << "Scheduler - task unknown exception" << endl; 
            }
        }
        this_thread :: sleep_for(milliseconds(5)); 
        // coarse- grained..// 
    }
}