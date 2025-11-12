#include "behavior_manager.h"
using namespace std; 

#include <fstream>
#include <iostream>

BehaviorManager :: BehaviorManager()
{
    sandbox = make_shared<BehaviorSandbox>(); 
    engine_ptr = make_unique<BehaviorEngine>();
    engine_ptr -> set_sandbox(sandbox); 
}

BehaviorManager :: ~BehaviorManager(){}

void BehaviorManager :: init(function <void(function<void()>)> async_poster)
{
    sandbox -> set_async_poster(async_poster);
}

void BehaviorManager :: load_rules(const vector<string>&lines)
{
    engine_ptr -> load_rules_from_lines(lines); 
}

bool BehaviorManager :: load_rules_from_file(const string &path, string &err)
{

    ifstream f(path); 
    if (!f.good()){
        err ="File not found:" + path; 
        return false; 
    }

    vector<string> lines; 
    string line; 
    while (getline(f, line))
    {
        line = line.empty() ? line : line; 
        if (!line.empty()) lines.push_back(line);
    }
    engine_ptr -> load_rules_from_lines(lines); 
    return true; 

}

void BehaviorManager :: register_control_handler(function<void(const string &payload)> handler)
{
    sandbox -> register_action_handler("SET_SPEED", handler); 
    sandbox -> register_action_handler("SET_FOLLOW_DISTANCE", handler); 
}

void BehaviorManager :: register_emergency_handler(function<void(const string &reason)> handler)
{
    sandbox->register_action_handler("EMERGENCY_BRAKE", [handler](const std::string& p){
        handler(p);
    });
    sandbox->register_action_handler("EMERGENCY_ALERT", [handler](const std::string& p){
        handler(p);
    });
}

vector<string> BehaviorManager :: evaluate_context(const unordered_map<string,string> &ctx)
{
    return engine_ptr -> evaluate_and_run(ctx); 
}

BehaviorEngine & BehaviorManager :: engine()
{
    return *engine_ptr; 
}

