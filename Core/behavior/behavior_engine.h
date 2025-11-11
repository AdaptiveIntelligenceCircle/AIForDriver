#pragma once 
#include "behavior_rule.h"

#include "behavior_sandbox.h"
#include <vector> 

#include <mutex>
#include <optional>

using namespace std; 
#include <unordered_map>

#include <algorithm> 
#include <memory> 

class BehaviorEngine
{
    public: 
    enum class Mode
    {
        FIRST_MATCH, ALL_MATCHES
    }; 

    BehaviorEngine();
    ~BehaviorEngine(); 

    // non - copyable 
    BehaviorEngine(const BehaviorEngine&) = delete; 
    BehaviorEngine &operator =(const BehaviorEngine&) = delete; 

    // load rules from list of DSL lines,, 
    void load_rules_from_lines(const vector<string> &lines); 

    // clear rules..
    void clear_rules(); 

    // set mode .. 
    void set_mode(Mode m); 

    // set sandbox to execute actions.. 
    void set_sandbox(shared_ptr<BehaviorSandbox> sb); 
    // evaluate given context and run actions (returns list of actions executed).. 
    vector<string> evaluate_and_run(const unordered_map<string, string> &context);
   
    // list loaded rule names.. 
    vector<string> list_rules() const; 

    private: 
    mutable mutex mtx; 
    vector<BehaviorRule> rules; 
    Mode mode; 
    shared_ptr<BehaviorSandbox> sandbox; 
}; 