#pragma once
#include <iostream>

#include <string> 
#include <cmath> 

#include <algorithm> 
#include <cmath> 

using namespace std; 

#include <optional>
#include <vector> 

#include <unordered_map>
#include <optional>

struct Condition
{
    string key; 
    string op; 
    string value; 
}; 

class BehaviorRule
{
    public: 
    BehaviorRule() = default; 
    ~BehaviorRule() {}

    BehaviorRule(string name, vector<Condition> conds, string action); 

    const string &name() const; 
    const vector<Condition> &conditions() const;
    const string &action() const; 

    // parse rule from single line DSL. Returns nullopt

    static optional<BehaviorRule> parse_from_dsl(const string &line ); 

    // Evaluate rule given a contact map : key -> string value.
    // numeric comparisons try stod, fallback to lexical compare..
    bool evaluate(const unordered_map<string, string> &context) const; 

    private: 
    string _name; 
    vector<Condition> _conds; 
    string _action; 

}; 