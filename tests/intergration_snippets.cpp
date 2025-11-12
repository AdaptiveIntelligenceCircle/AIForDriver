// // example_integration.cpp (pseudo)
// #include "behavior_manager.h"
// // #include "executor.h"
// #include <iostream>

// int main() {
//     Executor exec(2);
//     BehaviorManager bman;
//     bman.init([&exec](std::function<void()> fn){ exec.post(std::move(fn)); });

//     // register handlers
//     bman.register_control_handler([&](const std::string& payload){
//         std::cout << "[ControlHandler] payload=" << payload << std::endl;
//         // parse payload or call ControlChannel
//     });
//     bman.register_emergency_handler([&](const std::string& reason){
//         std::cerr << "[EmergencyHandler] reason=" << reason << std::endl;
//         // call EmergencyProtocol::trigger
//     });

//     // load some rules (DSL lines)
//     std::vector<std::string> rules = {
//         "IF distance_to_object < 5 THEN EMERGENCY_BRAKE",
//         "IF attention < 0.2 AND fatigue > 0.7 THEN EMERGENCY_ALERT",
//         "IF traffic_density > 0.8 THEN SET_FOLLOW_DISTANCE:25"
//     };
//     bman.load_rules(rules);

//     // simulate context
//     std::unordered_map<std::string, std::string> ctx = {
//         {"distance_to_object", "4.2"},
//         {"attention", "0.15"},
//         {"fatigue", "0.85"},
//         {"traffic_density", "0.9"}
//     };

//     auto executed = bman.evaluate_context(ctx);
//     for (auto &a : executed) std::cout << "[Main] executed action: " << a << std::endl;

//     exec.shutdown();
//     return 0;
// }
