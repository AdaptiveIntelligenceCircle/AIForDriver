#pragma once 
#include <string> 

#include <functional>
#include <mutex> 

#include <unordered_map>
#include <chrono>

using namespace std; 

class IntrusionDetector 
{
    public: 
    public:
    IntrusionDetector();
    ~IntrusionDetector();

    // Feed raw event (type, payload) into detector.
    void feed_event(const string& type, const string& payload);

    // Register callback for alerts (reason, severity [0-1])
    void register_alert_callback(function<void(const string&, double)> cb);

    // Query last trust score (1.0 = fully trusted, 0.0 = compromised)
    double trust_score() const;

    // Manual reduce/increase trust (for operator actions)
    void adjust_trust(double delta);

private:
    mutable mutex mtx;
    double trust; // 0..1
    function<void(const string&, double)> alert_cb;

    // internal simple counters
    unordered_map<string, int> recent_counts;
    chrono::steady_clock::time_point last_reset;

    void maybe_reset_counters();
    void raise_alert(const string& reason, double severity);
}; 