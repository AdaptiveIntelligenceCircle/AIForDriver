#include <iostream> 
#include <string> 

#include <unordered_map>
#include <chrono>

using namespace std; 

enum class DriverStatus 
{
    NORMAL, 
    DISTRACTED, 
    DROWSY,
    EMERGENCY
}; 

class DriverState
{
    private: 
    DriverStatus current_status; 
    unordered_map<string, double> v ; 

    public: 
    DriverState() : current_status(DriverStatus :: NORMAL){

    }

    void updateVital (const string &key, double value)
    {
        v[key] = value; // vitalSigns = value...// 
    }

    void evaluate()
    {
        if (v["attention"] < 0.3)
        current_status = DriverStatus :: DISTRACTED;
        if (v["Fatigue"] < 0.7) 
        current_status = DriverStatus :: DROWSY; 
        else current_status = DriverStatus :: NORMAL; 
        
        cout <<"DriverState - Status :" << static_cast<int>(current_status) << endl;
    }

    DriverStatus getStatus() const 
    {
        return current_status; 
    }
}; 