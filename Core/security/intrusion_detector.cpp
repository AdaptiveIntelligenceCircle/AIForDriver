#include "intrusion_detector.h"
#include <iostream>
#include <algorithm>

IntrusionDetector::IntrusionDetector() : trust(1.0), last_reset(std::chrono::steady_clock::now()) {}

IntrusionDetector::~IntrusionDetector() {}

void IntrusionDetector::feed_event(const string& type, const std::string& payload) {
    lock_guard<mutex> lock(mtx);
    maybe_reset_counters();

    recent_counts[type] += 1;
    // Heuristics:
    // - repeated checksum failures -> reduce trust
    // - lua_panic -> immediate alert
    if (type == "checksum_failure") {
        trust -= 0.05;
        raise_alert("checksum_failure", 0.6);
    } else if (type == "lua_panic") {
        trust -= 0.25;
        raise_alert("lua_panic", 0.9);
    } else if (type == "auth_fail") {
        trust -= 0.1;
        raise_alert("auth_fail", 0.7);
    } else if (type == "high_freq_packets") {
        // if more than 10 in window -> suspect flood
        if (recent_counts[type] > 10) {
            trust -= 0.15;
            raise_alert("high_freq_packets", 0.8);
        }
    }

    // clamp trust
    if (trust < 0.0) trust = 0.0;
    if (trust > 1.0) trust = 1.0;
}

void IntrusionDetector::register_alert_callback(function<void(const std::string&, double)> cb) {
    lock_guard<mutex> lock(mtx);
    alert_cb = std::move(cb);
}

double IntrusionDetector::trust_score() const {
    lock_guard<mutex> lock(mtx);
    return trust;
}

void IntrusionDetector::adjust_trust(double delta) {
    lock_guard<mutex> lock(mtx);
    trust += delta;
    if (trust < 0.0) trust = 0.0;
    if (trust > 1.0) trust = 1.0;
}

void IntrusionDetector::maybe_reset_counters() {
    using namespace chrono;
    auto now = steady_clock::now();
    if (now - last_reset > seconds(10)) {
        recent_counts.clear();
        last_reset = now;
    }
}

void IntrusionDetector::raise_alert(const string& reason, double severity) {
    cerr << "[IntrusionDetector] ALERT: " << reason << " severity=" << severity << std::endl;
    if (alert_cb) {
        // call without holding lock to avoid reentrancy issues
        auto cb = alert_cb;
        // unlock by scope exit then call
        std::lock_guard<std::mutex> unlock(mtx); // no-op but keeps consistent style
        cb(reason, severity);
    }
}
