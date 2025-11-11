#include "behavior_engine.h"
#include <iostream> 

using namespace std; 

BehaviorEngine::BehaviorEngine() : mode(Mode::FIRST_MATCH) {}
BehaviorEngine::~BehaviorEngine() {}

void BehaviorEngine::load_rules_from_lines(const vector<string>& lines) {
    lock_guard<mutex> lock(mtx);
    rules.clear();
    for (const auto& l : lines) {
        auto opt = BehaviorRule::parse_from_dsl(l);
        if (opt.has_value()) rules.push_back(*opt);
        else cerr << "[BehaviorEngine] Failed to parse rule: " << l << endl;
    }
}

void BehaviorEngine::clear_rules() {
    lock_guard<mutex> lock(mtx);
    rules.clear();
}

void BehaviorEngine::set_mode(Mode m) {
    lock_guard<mutex> lock(mtx);
    mode = m;
}

void BehaviorEngine::set_sandbox(shared_ptr<BehaviorSandbox> sb) {
    lock_guard<mutex> lock(mtx);
    sandbox = sb;
}

vector<string> BehaviorEngine::evaluate_and_run(const unordered_map<string, string>& context) {
    vector<string> executed;
    shared_ptr<BehaviorSandbox> sb_local;
    {
        lock_guard<mutex> lock(mtx);
        sb_local = sandbox;
        if (!sb_local) {
            cerr << "[BehaviorEngine] No sandbox configured; aborting action execution." << endl;
        }
        // evaluate rules
        for (const auto& r : rules) {
            if (r.evaluate(context)) {
                executed.push_back(r.action());
                if (sb_local) sb_local->execute_action(r.action());
                if (mode == Mode::FIRST_MATCH) break;
            }
        }
    }
    return executed;
}

vector<string> BehaviorEngine::list_rules() const {
    lock_guard<mutex> lock(mtx);
    vector<string> names;
    for (const auto& r : rules) names.push_back(r.name());
    return names;
}