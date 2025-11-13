#pragma once 
#include <functional>

#include <string> 
#include <mutex> 

using namespace std; 

class ModelUpdater
{
    public: 
    ModelUpdater(); 
    ~ModelUpdater(); 

    void update_threshold(const string &metric, double new_z); 
    double get_threshold(const string &metric) const; 

    void register_persist_hook(function<void(const string& /*metric*/, double /*val*/)> hook); 

    private: 
    mutex mutable mtx; 
    unordered_map<string, double> thresholds; 
    function<void(const string &, double)> persist_hook; 
};