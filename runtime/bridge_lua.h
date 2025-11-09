#pragma once 
#include <string> 
#include <algorithm>

#include <optional>
#include <mutex> 
#include <functional>

using namespace std; 

struct lua_State; // forward declare.. // 
class BridgeLua
{
    public: 
    BridgeLua(); 
    ~BridgeLua(); 

    // Non - copyable.. // 
    BridgeLua(const BridgeLua&) = delete; 
    BridgeLua operator=(const BridgeLua&) = delete; 

    // load a lua script from file.. // 
    bool load_script(const string &path, string &err);

    optional<std::string> call_global_stringfn(const std::string& func, const std::string& arg, std::string& err);
    // Call a global lua function with a single string arguement.. 
    optional<string> call_signal_stringfn(const string &func, const string &arg, string &err);
    
    // call ab global lua function with no arguements..
    optional<double> call_global_numberfn(const string &func, string &err);

    // register a C++ callback that Lua script can call (via name)
    void register_callback(const string &name, function<void(const string &)> cb);
    
    private: 

    lua_State *L; 
    mutex mtx; 
    int next_cb_ref; 

    // helper to push and pop errors;
    void push_error(const string &s); 
}; 