#pragma once 
#include <optional> 
#include <atomic> 

#include <thread> 
#include <chrono>

#include <vector> 
#include <mutex> 

#include <functional>
using namespace std; 

class ReactorLoop
{
    public: 
    using Callback = function<void()> ; 
    ReactorLoop(); 
    ~ReactorLoop(); 

    // Non - copyable .. // 
    ReactorLoop(const ReactorLoop&) = delete; 
    ReactorLoop &operator=(const ReactorLoop&) = delete; 

    // Register a periodic callback with period in milliseconds
    // returns an id that can be used to unregister.. / 
    int register_periodic(Callback cb, uint64_t period_ms, uint64_t intial_delayed); 

    // unregister a callback by id.. 
    void unregister(int id); 

    // start / stop reactor .. //
    void start(); 
    void stop(); 

    // check running 
    bool is_running() const; 

    private: 
    struct Item
    {
        int id; 
        Callback cb; 
        chrono :: steady_clock :: time_point next; 
        chrono :: milliseconds period; 
        bool cancelled; 
    }; 

    void loop_thread_fn(); 

    vector<Item> items; 
    mutex mtx; 
    atomic<bool> running; 
    thread loop_thread; 
    int next_id; 
}; 