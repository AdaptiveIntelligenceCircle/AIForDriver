#pragma once 
#include <string> 
#include <functional> 

class FailoverManager
{
    public: 
    FailoverManager(); 
    ~FailoverManager(); 

    // engage safe mode : disable lua, reduce speed,  etc.. 
    void engage_safe_mode(const string &reason); 

    // throttle channel for duration_ms//
    void throttle_noncritical(const string &channel, int duration_ms); 

    // register rollback hook.. 
    void set_rollback_hook(function<void(int snapshot_id)> cb); 

    // schedule rollback.. (non - blocking).. 
    void schedule_rollback(int snapshot_id); 

    private: 
    function<void(int)> rollback_hook; 
}; 