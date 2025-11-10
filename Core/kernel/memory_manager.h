#pragma once 
#include <string>

#include <unordered_map>
#include <mutex> 

#include <optional>
#include <chrono>

using namespace std; 

class MemoryManager
{
    public: 
    MemoryManager(); 
    ~MemoryManager(); 

    // save snapshot blob for a key, returns snapshot id (monotonic)
    int save_snapshot(const string &key, const string &blob)
    ; 
    // load lasted snapshot for a key (if any).. / 
    optional <string> load_lastest(const string &key); 
    
    // list snapshots for a key (ids)..// 
    vector<int> list_snapshots(const string &key); 

    // purge snapshots older than given seconds.. 
    void purge_older_than_seconds(int seconds); 

    // load by id : 
    optional<string> load_by_id(int snapshot_id); 

    

    private: 
    struct Snapshot
    {
        int id; 
        string key;
        string blob; 
        chrono :: steady_clock :: time_point ts; 
    }; 

    mutex mtx; 
    unordered_map<int, Snapshot> store; 
    unordered_map<string, vector<int>> index; 
    int next_id; 
}; 