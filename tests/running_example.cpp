// runtime_example.cpp
#include "../runtime/event_loop.h"
#include "../runtime/event_loop.cpp"
#include <iostream>

int main() {
    EventLoop el;
    el.init(4);
    el.start();

    // schedule a task to print message every 1s
    el.scheduler().schedule_periodic([&el]() {
        el.executor().post([]() {
            std::cout << "[Example] periodic task running." << std::endl;
        });
    }, std::chrono::seconds(1));

    // load a lua script (if exists)
    std::string err;
    if (el.lua_bridge().load_script("protocol/lua/adaptive_driver.lua", err)) {
        std::cout << "Loaded adaptive_driver.lua" << std::endl;
    } else {
        std::cout << "Lua load error: " << err << std::endl;
    }

    // run for 5 seconds then stop
    std::this_thread::sleep_for(std::chrono::seconds(5));
    el.stop();
    std::cout << "EventLoop stopped." << std::endl;
    return 0;
}
