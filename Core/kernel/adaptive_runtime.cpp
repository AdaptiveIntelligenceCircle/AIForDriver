#include "adaptive_runtime.h"
#include "reactor_loop.h"
#include "memory_manager.h"

#include <fstream> 
#include <iostream>

using namespace std ; 
#include <cmath>

AdaptiveRuntime :: AdaptiveRuntime(ReactorLoop *reactor_, MemoryManager *memory_)
: reactor(reactor_), memory(memory_)
{

}

AdaptiveRuntime :: ~AdaptiveRuntime()
{
    unload_behavior(); 
}

bool AdaptiveRuntime :: init(string &err)
{
    lock_guard<mutex> lock(mtx); 

    if (!reactor){
        err = "Reactor not provided"; 
        return false; 
    }

    if (!memory) {
        err = "Memory manager not provided"; 
        return false; 
    }
    // could register health checks in reactor..
    if (reactor -> is_running())
    {
        // optionally register health callback
    }
    return true; 
}

bool AdaptiveRuntime :: load_behavior_script(const string &script_path, string &err)
{
    lock_guard<mutex> lock(mtx); 
    // very small validation: file exists and reaable..
    ifstream f (script_path); 
    if (!f.good())
    {
        err = "script file not found:" + script_path; 
        return false;
    }
    // storing path; actual load 
    loaded_script = script_path; 
    cout <<"Adaptive Runtimr - Behavior script staged:" << script_path << endl; 
    return true; 
}

void AdaptiveRuntime :: unload_behavior()
{
    lock_guard<mutex> lock(mtx); 
    if (!loaded_script.empty())
    {
        cout <<"Adaptive Runtime - Unloading script:" << loaded_script << endl;
    }
    loaded_script.clear(); 
}

int AdaptiveRuntime :: save_checkpoint(const string &label)
{
    lock_guard<mutex> lock(mtx); 
    // create a simple snapshot containing metadata, in real worlds..
    string blob = string("Checkpoint:") + label + "; script = " + loaded_script; 
    if (!memory) return -1; 
    int id = memory -> save_snapshot("Adaptive_runtime", blob); 
    cout <<"[Adaptive Runtime] - Saved checkpoint id = " << id <<"Label = " << label << endl; 
    return id; 
}

bool AdaptiveRuntime :: restore_checkpoint(int snapshot_id, string &err)
{
    lock_guard<mutex> lock(mtx); 
    if (!memory) {
        err = "Memory not available"; 
        return false; 
    }
    auto opt = memory -> load_by_id(snapshot_id); 
    if (!opt.has_value())
    {
        err = "snapshot not found"; 
        return false; 
    }
    // naive restore : parse blob for script path
    string blob = opt.value(); 
    auto pos= blob.find("script="); 
    if (pos != string ::npos)
    {
        loaded_script = blob.substr(pos + 7); 
        cout <<"Adaptive Runtime - Restored script from snapshot" << loaded_script << endl;
    }
    else{
        loaded_script.clear(); 
    }
    return true; 
}

void AdaptiveRuntime::set_control_hook(function<void(const string&, const string&)> hook) {
    lock_guard<mutex> lock(mtx);
    control_hook = move(hook);
}

void AdaptiveRuntime::set_emergency_hook(function<void(const string&)> hook) {
    lock_guard<mutex> lock(mtx);
    emergency_hook = move(hook);
}

optional<string> AdaptiveRuntime::current_script() const {
    lock_guard<mutex> lock(mtx);
    if (loaded_script.empty()) return nullopt;
    return loaded_script;
}