#pragma once 
#include <iostream>
#include <string>
#include <functional>

#include <memory>
#include <atomic> 
using namespace std; 

struct SensorSnapshot;
struct DriverSnapshot; 

class SafetyMonitor; 
class HarzardAssessor; 
class EmergencyHandler; 
class WatchDog; 

class FailoverManager; 
class SafetyPolicy; 
class Validator; 

class SafetyManager
{
    public:
    SafetyManager();
    ~SafetyManager() = default; 

    // Non - copyable .. 
    SafetyManager (const SafetyManager&) = delete;
    SafetyManager &operator =(const SafetyManager&) = delete; 

    // lifecycle.. 
    void start(); 
    void stop(); 

    // ingest sensor + driver + misc metrics each tick.. 
    void ingest(const SensorSnapshot &s, const DriverSnapshot &d); 

    // force an emergency externally.. 
    void force_emergency(const string &reason); 

    // register a callback to notify external system.. 
    void register_emergency_callback(function<void(const string &)> cb);

    private: 
    unique_ptr<SafetyMonitor> monitor; 
    unique_ptr<HarzardAssessor> assessor; 
    unique_ptr<EmergencyHandler> handler;
    unique_ptr<WatchDog> watchdog;
    unique_ptr<FailoverManager> failover;
    unique_ptr<SafetyPolicy> policy;
    unique_ptr<Validator> validator;

    function<void(const string &)> external_emergency_cb; 
    atomic<bool> running; 

}; 