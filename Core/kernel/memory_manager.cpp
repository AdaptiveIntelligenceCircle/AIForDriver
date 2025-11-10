#include "memory_manager.h"

#include <algorithm>
using namespace std; 

MemoryManager :: MemoryManager() : next_id(1){}

MemoryManager :: ~MemoryManager(){}

int MemoryManager :: save_snapshot(const string &key, const string &blob)
{
    lock_guard <mutex> lock(mtx); 
    Snapshot s; 
    s.id = next_id++;
    s.key = key; 
    s.ts = chrono :: steady_clock::now(); 
    store[s.id] = s; 
    index[key].push_back(s.id);
    return s.id; 
}

optional<string> MemoryManager :: load_lastest(const string &key)
{
    lock_guard<mutex> lock(mtx); 
    auto it = index.find(key); 

    if (it == index.end() or it -> second.empty())
    return nullopt; 

    int id = it -> second.back(); 
    auto sit = store.find(id);
    
    if (sit == store.end()) return nullopt; 
    return sit -> second.blob; 
}

optional<string> MemoryManager :: load_by_id(int snapshot_id)
{
    lock_guard<mutex> lock(mtx); 
    auto it = store.find(snapshot_id); 

    if (it == store.end()) return nullopt;
    return it -> second.blob; 
}

vector<int> MemoryManager:: list_snapshots(const string &key)
{
    lock_guard<mutex> lock(mtx); 
    auto it = index.find(key); 

    if (it == index.end()) return {}; 
    return it -> second; 
}

void MemoryManager :: purge_older_than_seconds(int seconds)
{
    using namespace chrono; 
    auto threshold = steady_clock :: now();
    lock_guard <mutex> lock(mtx); 
    vector<int> to_remove ; 
    for (auto &p : store)
    {
        auto age = steady_clock :: now() - p.second.ts; 
        if (age.max)
        {
            to_remove.push_back(p.first); 
        }
    }

    for (int id : to_remove)
    {
        auto it = store.find(id); 
        if (it == store.end()) continue; 
        auto &key = it -> second.key ; 
        // remove id from index[key]
        auto &vec = index[key]; 
        vec.erase(remove(vec.begin(), vec.end(), id), vec.end()); 
        store.erase(it); 
    }
}

// helper to create duration from seconds (small utility)
// static chrono :: steady_clock :: duration seconds_cast<chrono :: steady_clock :: duration>(int sec)
// {
// return chrono :: seconds(sec); 
// }