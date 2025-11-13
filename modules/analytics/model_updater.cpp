#include "model_updater.h"

ModelUpdater::ModelUpdater() {}
ModelUpdater::~ModelUpdater() {}

void ModelUpdater::update_threshold(const string& metric, double new_z) {
    {
        lock_guard<mutex> lock(mtx);
        thresholds[metric] = new_z;
    }
    if (persist_hook) {
        try { persist_hook(metric, new_z); } catch (...) {}
    }
}

double ModelUpdater::get_threshold(const string& metric) const {
    lock_guard<mutex> lock(mtx);
    auto it = thresholds.find(metric);
    if (it == thresholds.end()) return 3.0; // default
    return it->second;
}

void ModelUpdater::register_persist_hook(function<void(const string&, double)> hook) {
    lock_guard<mutex> lock(mtx);
    persist_hook = move(hook);
}
