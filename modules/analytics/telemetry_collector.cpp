#include "telementry_collector.h"
#include <iostream> 

using namespace std; 
#include <string> 

TelementaryCollector :: TelementaryCollector(int max_windows_seconds)
: window_seconds(max_windows_seconds){

}

TelementaryCollector :: ~TelementaryCollector(){
    // 
}

void TelementaryCollector :: push(const string &sensor, double value)
{
    auto ts = now(); 
    lock_guard<mutex> lock(mtx); 
    auto &dq = storage[sensor]; 
    dq.push_back({ts, value}); 
    prune_old(dq); 
}

vector<TelementaryCollector :: Sample> TelementaryCollector :: get_window(const string &sensor) const 
{
    lock_guard<mutex> lock(mtx); 
    vector<Sample> out;
    auto it = storage.find(sensor);

    if (it == storage.end()) return out; 
    for (const auto &s : it -> second) out.push_back(s); 

    // return as - is (oldest -> newest)
    return out; 
}

void TelementaryCollector :: clear_sensor(const string &sensor)
{
    lock_guard<mutex> lock(mtx); 
    storage.erase(sensor); 
}

void TelementaryCollector ::  set_window_seconds(int seconds)
{
    lock_guard<mutex> lock(mtx); 
    window_seconds = seconds; 
    
    // prune all.. 
    for (auto &p : storage) prune_old(p.second); 
}

void TelementaryCollector :: prune_old(deque<Sample> &dq)
{
    auto cutoff = now() - chrono :: seconds(window_seconds); 
    while (!dq.empty() and dq.front().ts < cutoff) 
    dq.pop_front(); 
}