#pragma once 
#include <functional>

#include <future> 
#include <queue> 

#include <mutex> 
#include <thread> 

#include <atomic> 
#include <vector> 

#include <condition_variable>
#include <optional>

using namespace std; 

// Executor : thread - pool like simple executor 
// - push tasks (function<void()>)
// support submit task that returns future.. ..

class Executor
{
    public: 
    Executor(size_t worker_count = thread::hardware_concurrency()); 
    ~Executor(); 

    // Non - copyable 
    Executor(const Executor &) = delete; 
    Executor  &operator = (const Executor&) = delete; 

    // submit a fire - and - forget task..
    void post(function<void()> task); 

    // submit a task returning future<R> 

    template<typename F, typename... Args> 
    auto submit(F &&f, Args&&... args) -> 
    future<decltype(f(args...))>;

    // Stop and join workers.. // 
    void shutdown(); 

    private: 
    void worker_loop(); 

    vector<thread> workers; 
    queue<function<void()>> tasks; 
    mutex mtx; 
    condition_variable cv; 
    atomic<bool> running; 
}; 