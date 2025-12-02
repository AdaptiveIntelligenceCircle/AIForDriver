#include "safety_log.h"
using namespace std; 

#include <iostream> 
#include <chrono>

#include <iomanip> 
#include <vector> 

static string now_ts()
{
    using namespace chrono; 
    auto t = system_clock:: now(); 

    time_t tt = system_clock :: to_time_t(t); 
    tm tm{}; 

    #ifdef _WIN32
    localtime_s(&tm, &tt); 
    #else 
    localtime_r(&tt, &tm); 
    #endif 
    char buf[32]; 
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S",  &tm); 
    return string(buf) ; 
}

SafetyLog & SafetyLog :: instance()
{
    static SafetyLog s;
    return s; 
}

void SafetyLog :: info(const string &msg)
{
    cout <<"Info" << msg << endl; 
    cout << now_ts() ; 
}

void SafetyLog :: warn(const string &msg)
{
    cout << "Warn" << msg << endl;
    cout << now_ts();  
} 

void SafetyLog :: error(const string &msg)
{
    cout <<"Error" << msg << endl;
    cout << now_ts();  
}

void SafetyLog :: debug(const string &msg)
{
    cout << "Debug" << msg << endl;
    cout << now_ts();  
}

void SafetyLog::log_event(const string& name, const unordered_map<string,string>& kv) 
{
    cout << "[EVENT][" << now_ts() << "][" << name << "] ";
    for (auto &p : kv) cout << p.first << "=" << p.second << " ";
    cout << endl;
}