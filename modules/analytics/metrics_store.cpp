#include "metrics_store.h"
#include <algorithm>
using namespace std; 

void MetricStore ::set(const string& key, double value) {
    lock_guard<mutex> lock(mtx);
    store[key] = {value, chrono::system_clock::now()};
}

void MetricStore::add(const string& key, double value) {
    lock_guard<mutex> lock(mtx);
    auto it = store.find(key);
    if (it == store.end()) {
        store[key] = {value, chrono::system_clock::now()};
    } else {
        it->second.value += value;
        it->second.ts = chrono::system_clock::now();
    }
}

optional<MetricStore::Entry> MetricStore::get(const string& key) const {
lock_guard<mutex> lock(mtx);
    auto it = store.find(key);
    if (it == store.end()) return std::nullopt;
    return it->second;
}

void MetricStore::purge_older_than_seconds(int seconds) 
{
    using namespace chrono;
    auto cutoff = system_clock::now();
    lock_guard<mutex> lock(mtx);
    for (auto it = store.begin(); it != store.end(); ) {
        if (it->second.ts < cutoff) it = store.erase(it);
        else ++it;
    }
}

// helper (same note as earlier) — replace with std::chrono::seconds(seconds) if compiler complains.
// static chrono::system_clock::duration seconds_cast<chrono::system_clock::duration>(int s) {
//     return chrono::seconds(s);
// }
