#pragma once 
#include <functional> 
#include <string>
#include <mutex> 

using namespace std ; 

class BehaviorSandbox
{
    public: 
    BehaviorSandbox(); 
    ~BehaviorSandbox(); 

    // register an action handler by name..//
    void register_action_handler(const string &action_name, function<void(const string &payload)> handler); 

    // set the async poster .. 
    void set_async_poster(function<void(function<void()>)> poster); 

    // execute action string, parsed as "Name : payload"
    void execute_action(const string &action); 

    private: 
    mutex mtx; 
    function<void(function<void()>)> poster; 
    unordered_map<string, function<void(const string &)>> handlers; 
}; 

