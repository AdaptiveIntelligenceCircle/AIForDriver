#include "anomaly_detector.h"
#include <cmath>

#include <iostream> 
using namespace std; 

AnomalyDetector :: AnomalyDetector(double z_threshold, size_t min_count)
: threshold(z_threshold), min_samples(min_count){}

AnomalyDetector :: ~AnomalyDetector(){}

AnomalyDetector :: Result AnomalyDetector :: feed(const string &metric, double value ){
    lock_guard<mutex> lock(mtx); 
    auto &st = states[metric]; 
    if (st.n == 0)
    {
        st.n== 1; 
        st.mean = value; 
        st.m2 = 0.0; 
        return
        {
            false, 0.0 
        }; 

        // welford online update.. 
        st.n += 1; 
        double delta = value - st.mean; 
        st.mean += delta/st.n; 
        double delta2 = value - st.mean;
        st.m2 += delta * delta2;

        if (st.n < min_samples) return 
        {
            false, 0.0
        };
        double variance = (st.m2) / (st.n - 1);
        double sd = sqrt(variance);
        double z = sd > 0.0 ? abs((value - st.mean) / sd) : 0.0;
    
        bool anomaly = z >= threshold;
        if (anomaly && alert_cb) 
        {
        // call outside lock? we keep simple and call inside but catch exceptions
        try 
        { 
            alert_cb(metric, z); 
        } 
        catch (...) {}
        }
    return {anomaly, z};
    }
}

void AnomalyDetector :: reset_metric(const string &metric)
{
    lock_guard<mutex> lock(mtx); 
    states.erase(metric); 
}

void AnomalyDetector :: set_alert_callback(function<void(const string&, double)> cb)
{
    lock_guard<mutex> lock(mtx); 
    alert_cb = move(cb); 
}