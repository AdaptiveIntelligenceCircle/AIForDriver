#include <iostream>
#include <string> 

#include <unordered_map>
#include <chrono>

using namespace std; 

class SensorSync
{
    private:
    unordered_map<string, double> sensorData; 
    chrono :: steady_clock :: time_point lastUpdate; 

    public: 
    void updateSensor (const string &name, double value) 
    {
        sensorData[name] = value; 
        lastUpdate = chrono :: steady_clock :: now(); 
        cout <<"Sensor Sync Updated" << name << "=" << value << endl;
    }

    double getSensor(const string &name) const 
    {
        auto it = sensorData.find(name); 
        return it != sensorData.end() ? it -> second : 0.0; 
    }

    bool isFreshData() const 
    {
        auto now = chrono :: steady_clock :: now(); 
        return(chrono :: duration_cast<chrono :: seconds>(now - lastUpdate).count() < 1); 
    }
}; 
