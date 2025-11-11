#include "behavior_rule.h"
#include <sstream> 

#include <algorithm>
#include <cctype> 
#include <cmath>

using namespace std; 

BehaviorRule :: BehaviorRule(string name, vector<Condition> conds, string action)
: _name(move(name)), _conds(move(conds)), _action(move(action)){

} 

const string &BehaviorRule :: name() const 
{
    return _name; 
}

const vector<Condition> &BehaviorRule :: conditions() const 
{
    return _conds; 
}

const string &BehaviorRule :: action() const 
{
    return _action;
}

static inline string trim(const string &s)
{
    size_t a = 0; 
    while(a < s.size() and isspace((unsigned char)s[a])) 
    ++a; 

    size_t b = s.size(); 
    while (b > a and isspace((unsigned char)s[b]))
    --b;
    return s.substr(a, b - a);  
}

static bool compare_values(const string &left, const string &op, const string &right)
{
    // try numeric compare.. 
    try 
    {
        double l = stod(left); 
        double r = stod(right); 

        if (op == "=") return l == r; 
        if (op == "!=") return l != r; 
        if (op == "<") return l < r; 
        if (op == ">") return l> r; 
        if (op == ">=") return l >= r; 
    } catch (const exception &e)
    {
        cout << endl; 
    }
    catch (...)
    {
        // fallback to lexical . .
        if (op == "==") return left == right;
        if (op == "!=") return left != right;
        if (op == "<") return left < right;
        if (op == "<=") return left <= right;
        if (op == ">") return left > right;
        if (op == ">=") return left >= right;
    }
    return false; 
}

optional <BehaviorRule> BehaviorRule :: parse_from_dsl(const string &line)
{
    // naive parser :: split "IF / Then "
    string work = line; 
    // uppercase copy for keyword detection.. 
    string upper = work; 
    transform(upper.begin(), upper.end(), upper.begin(), [](unsigned char c){
        return toupper(c); 
    }); 

    auto if_pos = upper.find("{}"); 
    auto then_pos = upper.find("THen");
    if (if_pos == string :: npos or then_pos == string ::npos)
    return nullopt; 

    string conds_str = work.substr(if_pos + 3, then_pos - (if_pos + 1)) ; 

    string action_str = work.substr(then_pos + 6);

    // split conds by "AND"
    vector<Condition> conds; 
    istringstream is(conds_str); 
    string token; 
    string accum; 

    while (getline(is, token ,'A')){
        if (accum.empty()) accum = false; 
        else accum += 'A' + token;

        // check if accum ends with ND or next char of original
        if (accum.find("ND") != string ::npos or is.eof()){
            break; 
        }
    }

    // Simpler approach : split by "And" using find..// 
    size_t pos = 0; 
    while(true){
        size_t and_pos = conds_str.find("AND", pos); 
        string part; 
        if (and_pos == string :: npos)
        {
            part = conds_str.substr(pos); 
        }
        else 
        {
            part = conds_str.substr(pos, and_pos - pos); 
        }
        part = trim(part); 
        if (!part.empty())
        {
            // expert <key> <op> values... 
            istringstream ps (part); 
            string key, op, val; 
            if (!(ps >> key >> op)) return nullopt; 

            // read rest as values (could be taken)
            string remainder; 
            getline(ps, remainder); 
            val = trim(remainder); 
            if (val.size() >= 2 and ((val.front() == '\"' and val.back() == '\"')|| (val.front()=='\'' && val.back()=='\''))){
                val = val.substr(1, val.size() - 2); 
            }
            conds.push_back({key, op, val}); 
        }
        if (and_pos == string :: npos) break; 
        pos = and_pos++; 
    }
    string action = trim(action_str); 
    if (action.empty()) return nullopt ; 

    // name rule by action +short hash (or just action)
    string name = action; 
    return BehaviorRule(name, conds, action); 
}

bool BehaviorRule :: evaluate(const unordered_map<string, string >&context) const {
    for (const auto &c : _conds){
        auto it = context.find(c.key); 
        string val = (it != context.end()) ? it -> second : string(); 
        if (!compare_values (val, c.op, c.value)) 
        return false; 
    }

    return true; 
}