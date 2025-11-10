#pragma once 
#include <functional>

#include <string> 
#include <mutex> 

#include <optional>
using namespace std; 

class ReactorLoop; 
class MemoryManager; 

class AdaptiveRuntime
{
    public: 
    AdaptiveRuntime(ReactorLoop * reactor, MemoryManager *memory); 
    ~AdaptiveRuntime();

    // Initializer runtime (load default profile).. 
    bool init(string &err); 

    // Load adaptive behavior code .. 
    bool load_behavior_script(const string &script_path, string &err); 

    // unload behavior..
    void unload_behavior(); 

    // Request save of runtime checkpoint(returns snapshot id)
    int save_checkpoint(const string &label); 

    // Restore checkpoint by id ,, 
    bool restore_checkpoint(int snapshot_id, string &err); 

    // set holds to call for control/emergency 
    void set_control_hook(function<void(const string& cmd, const string& payload)> hook); 
    void set_emergency_hook(function<void(const string &reason) > hook); 

    // Query current loaded script path.. 
    optional<string> current_script() const; 

    private: 
    ReactorLoop *reactor; 
    MemoryManager *memory; 
    string loaded_script; 
    mutable mutex mtx; 

    function<void(const string &, const string &)> control_hook; 
    function<void(const string&)> emergency_hook; 
};  