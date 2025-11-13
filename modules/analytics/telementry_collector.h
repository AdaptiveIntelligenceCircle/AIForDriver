#pragma once 
#include <string> 
#include <unordered_map>

#include <deque> 
#include <mutex>

#include <chrono>
#include <vector>

using namespace std; 

class TelementaryCollector
{
    public: 
    using TimePoint = chrono :: steady_clock :: time_point; 
    struct Sample
    {
        TimePoint ts; 
        double value; 
    }; 

    TelementaryCollector(int max_window_seconds = 10);
    ~TelementaryCollector(); 

    // ingest a value .//
    void push(const string &sensor, double value); 

    // get copy of samples for a sensor in the window
    vector<Sample> get_window(const string &sensor) const; 

    // clear sensor data.. // 
    void clear_sensor(const string &sensor); 

    // set window size(seconds)//
    void set_window_seconds(int seconds); 

    private: 
    mutable mutex mtx; 
    int window_seconds; 
    unordered_map<string, deque<Sample>> storage; 
    
    void prune_old(deque<Sample> &dq); 
    TimePoint now() const 
    {
        return chrono :: steady_clock :: now(); 
    }
}; 