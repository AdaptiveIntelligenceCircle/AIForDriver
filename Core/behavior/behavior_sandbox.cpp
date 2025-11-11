#include "behavior_sandbox.h"
#include <iostream> 
using namespace std; 

BehaviorSandbox :: BehaviorSandbox(){}
BehaviorSandbox :: ~BehaviorSandbox(){}

void BehaviorSandbox :: register_action_handler(const string &action_name, function<void(const string &payload) > handler )
{
    lock_guard<mutex> lock(mtx); 
    handlers[action_name] = move(handler); 
}

void BehaviorSandbox :: set_async_poster(function<void(function<void()>) > poster)
{
    lock_guard<mutex> lock(mtx); 
    poster = move(poster) ; 
} 

void BehaviorSandbox :: execute_action(const string &action)
{
    string name = action; 
    string payload; 
    size_t colon = action.find(":"); 

    if (colon != string :: npos)
    {
        name = action.substr(0, colon); 
        payload = action.substr(colon + 1); 
    }
    
    function<void(const string &)> handler;
    {
        lock_guard<mutex> lock(mtx); 
        auto it = handlers.find(name); 
        if (it != handlers.end()) handler = it -> second ; 
    }

    if (!handler){
        cout << "No handler for action:" << name << endl; 
        return; 
    }

    // if poster provided, run async, otherwise call sync.. 
    function<void(function<void()>)> p; 
    {
        lock_guard<mutex> lock(mtx);
        p = poster; 
    }

    if (p)
    {
        p([handler, payload]() {
            try { handler(payload); }
            catch (const std::exception& e) { std::cerr << "[BehaviorSandbox] handler exception: " << e.what() << std::endl; }
            catch (...) { std::cerr << "[BehaviorSandbox] handler unknown exception\n"; }
        });
    } else {
        try { handler(payload); }
        catch (const std::exception& e) { std::cerr << "[BehaviorSandbox] handler exception: " << e.what() << std::endl; }
        catch (...) { std::cerr << "[BehaviorSandbox] handler unknown exception\n"; }
    }
    
}