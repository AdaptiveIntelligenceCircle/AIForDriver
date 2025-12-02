#pragma once 
using namespace std ; 

#include <iostream> 
#include <functional> 

#include <unordered_map>
#include <chrono>

#include <thread>
#include <atomic>

class Watchdog
{
    public: 
    Watchdog(); 
    ~Watchdog(); 

    // start / stop 
    void start(); 
    void stop(); 

    void beat(const string &source); 

    // set timeout callback.. 
    void set_timeout_callback(function<void(const string &) >cb);
    
    private: 
    mutex mtx;
    unordered_map <string, chrono :: steady_clock :: time_point> last_beat; 
    function<void(const string &)> timeout_cb; 

    atomic <bool> running; 
    thread loop_thread; 
    chrono :: milliseconds poll_interval{200}; 
    chrono :: milliseconds timeout_threshild{1500}; 

    void loop_fn(); 
}; 