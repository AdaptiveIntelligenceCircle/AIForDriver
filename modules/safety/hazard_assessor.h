#pragma once 
#include <string>

#include <iostream>
using namespace std; 

class HazardAssessor
{
    public: 
    enum class Level {
        OK, WARNING, CRITICAL
    }; 

    struct Result
    {
        double score; 
        Level level; 
        string reason; 

        string level_str() const
        {
            switch(level){
                case Level :: OK : return "OK"; 
                case Level :: WARNING : return "WARNING"; 
                case Level :: CRITICAL : return "CRITICAL"; 
            }
            return "UNKNOWN"; 
        }
    };

    HazardAssessor (); 
    ~HazardAssessor(); 

    // Assess by (type, numeric value).. 
    Result assess(const string &type, double value); 

    // Assess intrusion events;
    Result assess_intrusion(const string &reason, double severity); 

    private: 
    double weigh(const string &type, double value); 
}; 