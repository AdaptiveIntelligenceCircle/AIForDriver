#pragma once 
#include <string> 
#include <unordered_map>
#include <mutex> 
#include <functional>

using namespace std; 

class AnomalyDetector 
{
    public: 
    struct Result
    {
        bool is_anomaly; 
        double score; 
    }; 

    AnomalyDetector(double z_threshold = 3.0, size_t min_count = 5); 
    ~AnomalyDetector(); 
    // Feed new observation, returns detecttion result//
    Result feed(const string &metric, double value); 
    
    //Reset metrics state..// 
    void reset_metric(const string &metric); 

    // register callback for anomaly events 
    void set_alert_callback(function<void(const string&, double)> cb); 

    private: 
    struct State
    {
        size_t n; 
        double mean; 
        double m2; 
    }; 

    mutex mtx;
    unordered_map<string, State> states; 
    double threshold; 
    size_t min_samples; 
    function<void(const string &, double)> alert_cb; 

}; 