#include "feature_extractor.h"
#include <numeric>
#include <cmath> 
using namespace std; 

FeatureExtractor :: FeatureMap FeatureExtractor :: extract_all(const TelementaryCollector &collector)
const 
{
    FeatureMap out; 
    return out; 
}

FeatureExtractor :: FeatureMap FeatureExtractor::extract_from_window(const string &sensor, vector<TelementaryCollector :: Sample> &window) const{
    FeatureMap out; 
    if (window.empty()) return out; 

    vector<double> vals; 
    vals.reserve(window.size()); 
    for (const auto &s: window) 
    vals.push_back(s.value); 

    double mu = mean(vals);
    double sd = stddev(vals, mu); 
    double mn = minval(vals); 
    double mx = maxval(vals); 

    double sl = slope(window);
    double last = vals.back(); 
    size_t cnt = vals.size(); 

    out[sensor + ".mean"] = mu; 
    out[sensor + ".std"] = sd; 
    out[sensor + ".min"] = mn; 
    out[sensor + ".max"] = mx;
    out[sensor + ".slope"] = sl;
    out[sensor + ".last"] = last;
    out[sensor + ".count"] = static_cast<double>(cnt);

    return out;
}

double FeatureExtractor :: mean(const vector<double> &v) const
{
    if (v.empty()) return 0.0; 
    double s = accumulate(v.begin(), v.end(), 0.0); 
    return s/v.size(); 
}

double FeatureExtractor :: stddev(const vector<double> &v, double mu) 
const {
    if (v.size() < 2) return 0.0; 
    double acc = 0.0; 
    for (double x : v ) acc += (x - mu) *(x -mu); 
    return sqrt(acc/(v.size() - 1)); 
}

double FeatureExtractor :: minval(const vector<double> &v) const 
{
    if (v.empty()) return 0.0; 
    return *std :: min_element(v.begin(), v.end()); 
}

double FeatureExtractor :: maxval(const vector<double>&v) const 
{
    if (v.empty()) return 0.0; 
    return *std ::max_element(v.begin(), v.end()); 
}

double FeatureExtractor :: slope(const vector<TelementaryCollector :: Sample> &s) const 
{
    if (s.size() < 2) return 0.0;
    // compute regression slope : cov(t,v) / var(t).. 
    double t0 = chrono::duration<double>(s.front().ts.time_since_epoch()).count();
    vector<double> tv;
    tv.reserve(s.size()); 
    for (const auto &sam : s){
        double ti = chrono::duration<double>(sam.ts.time_since_epoch()).count() - t0;
        tv.push_back(ti); 
    }

    // values 
    vector<double> vals;
    vals.reserve(s.size()); 
    for (const auto &sam :s) vals.push_back(sam.value); 

    double mean_t = mean(tv);
    double mean_v = mean(vals); 
    double cov = 0.0; 
    
    double var_t = 0.0; 

    for (size_t i = 0; i< tv.size(); ++i)
    {
        cov += (tv[i] - mean_t) *(vals[i] -mean_v); 
        var_t += (tv[i] -mean_t) *(tv[i] - mean_t); 
    }

    if (var_t == 0.0) return 0.0; 
    return cov/var_t; 
}