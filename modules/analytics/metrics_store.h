#pragma once 
#include <string> 
#include <unordered_map>
#include <mutex> 
#include <chrono> 
#include <optional> 
using namespace std; 

class MetricStore
{
    public: 
    struct Entry
    {
        double value; 
        chrono :: system_clock :: time_point ts; 
    };

    MetricStore() = default; 
    ~MetricStore() = default; 

    // set metric (overswrites)
    void set(const string &key, double value); 

    // update metric by adding value (accumulate)
    void add(const string &key, double value); 

    // get metric
    optional<Entry> get(const string &key) const; 
    void purge_older_than_seconds(int seconds); 

    private: 
    mutable mutex mtx; 
    unordered_map<string, Entry> store; 
};