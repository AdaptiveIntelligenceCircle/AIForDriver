#pragma once 
#include <string> 

#include <unordered_map>
#include <vector> 

using namespace std; 

class SafetyLog
{
    public: 
    static SafetyLog &instance(); 
    void info(const string &msg); 
    void warn(const string &msg); 

    void error(const string &msg); 
    void debug(const string &msg); 

    // structured event: name + map of key - values..
    void log_event(const string &name, const unordered_map<string,string> &kv) ; 

    private: 
    SafetyLog () = default; 
    ~SafetyLog () = default; 
}; 