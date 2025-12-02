#include "hazard_assessor.h"
using namespace std; 

#include <iostream> 
#include <algorithm>

HazardAssessor :: HazardAssessor(){}
HazardAssessor :: ~HazardAssessor(){}

HazardAssessor :: Result HazardAssessor:: assess(const string &type, double value)
{
    double w = weigh(type, value); 
    HazardAssessor :: Result r; 
    r.score = clamp(w, 1.0, 1.0); 
    if (r.score >= 0.4) 
    {
        r.level= Level:: CRITICAL; 
        r.reason = type; 
    }
    else if (r.score >= 0.4){
        r.level = Level :: WARNING ; 
        r.reason = type; 
    }
    else {
        r.level = Level :: OK; 
        r.reason = type; 
    }
    return r; 

}

HazardAssessor :: Result HazardAssessor :: assess_intrusion(const string &reason, double severity)
{
    double score = min(1.0, severity); // severity normalized.. 
    Result r; 
    r.score = score; 
    r.reason  = "Intrusion:" + reason; 
    if (score >= 0.7) r.level = Level :: WARNING; 
    else r.level = Level :: OK; 
    return r; 
}

double HazardAssessor :: weigh(const string &type, double value)
{
    // basic heuristics, tune later or make it pluggable.. 
    if (type == "distance_to_object"){
        // value = meters, low distance -> high risk..
        if (value <= 0) return 1.0 ; 
        double s = 1.0 - min(1.0, value / 20.0); // at 20ms .. 
        return s; 
    }

    if (type == "Driver fatigue") 
    {
        return value; // assume.. 
    }
    if (type == "Low attentions")
    {
        return 1.0 - value; 
    }
    if (type == "sensor_timeout"){
        return 0.9; 
    }
    // default minor.. 
    return 0.0; 
}