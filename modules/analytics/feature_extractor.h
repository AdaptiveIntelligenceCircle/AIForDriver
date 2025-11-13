#pragma once 
#include <string>

#include <unordered_map>
#include <vector> 

using namespace std; 
#include "telementry_collector.h"

class FeatureExtractor 
{
    public: 
    FeatureExtractor() = default; 

    using FeatureMap = unordered_map<string, double>;

    // compute features for all sensors found in collector.. 
    FeatureMap extract_all(const TelementaryCollector &collector) const; 

    // compute features for a single sensor window.. 
    FeatureMap extract_from_window(const string &sensor, vector<TelementaryCollector :: Sample> &window ) const;

    private: 
    double mean(const vector<double> &v)const; 
    double stddev(const vector<double> &v, double mu) const; 
    double minval(const vector<double> &v) const; 
    double maxval(const vector<double> &v) const; 

    // slope via simple linear regression (time as value)..
    double slope(const vector<TelementaryCollector :: Sample> &s) const; 
};  