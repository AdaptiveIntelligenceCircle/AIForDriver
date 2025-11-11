#pragma once 
#include <memory> 
#include <string>
using namespace std; 

#include "behavior_engine.h"
#include "behavior_sandbox.h"

class BehaviorManager 
{
    public: 
    BehaviorManager(); 
    ~BehaviorManager(); 

    // initialize with executor poster (e.g , eventloop.executor().post),

    void init(function<void(function<void()>)> async_poster); 

    // load rules from vector lines or now from file .. 
    void load_rules(const vector<string> &lines); 
    bool load_rules_from_file(const string &path, string &err); 

    // register core action handlers../
    void register_control_handler(function<void(const string &payload)> handler); 
    void register_emergency_handler(function<void(const string &reason)> reason); 

    // run evaluation with a context map.. / 
    vector<string> evaluate_context(const unordered_map<string,string> &ctx );

    // access engine.. 
    Behavior_engine &engine(); 

    private: 
    shared_ptr<Behavior_engine> engine_ptr; 
}; 