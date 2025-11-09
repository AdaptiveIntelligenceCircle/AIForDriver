#include "executor.h"
#include <iostream>

using namespace std; 

Executor :: Executor(size_t worker_count) : running(true)
{
    if (worker_count == 0) 
    worker_count = 1 ;
    for (size_t i = 0; i< worker_count; ++i)
    {
        workers.emplace_back([this]()
        {
            worker_loop(); 
        }); 
    }

}

Executor :: ~Executor()
{
    shutdown(); 
}

void Executor :: post(function<void()> task)
{
    {
        lock_guard<mutex> lock(mtx); 
        tasks.push(move(task)); 
    }
    cv.notify_one(); 
}

template <typename F, typename... Args> 
auto Executor :: submit(F&& f, Args&&... args) 
-> future<decltype(f(args...))> 
{
    using R = decltype(F(args...)); 
    auto task = make_shared<packaged_task<R()>>(
        bind(forward<F>(F), forward<Args>(args)...)
    ); 

    future <R> result = task -> get_future(); 
    post([task]()
    {
        (*task) (); 
    }); 
    return result;
}

// explicit template instantiation 

void Executor :: shutdown()
{
    bool expected = true;
    if (running.compare_exchange_strong(expected, false)){
        cv.notify_all(); 
        for (auto &t : workers)
        {
            if (t.joinable()) 
            t.join(); 
        }
        workers.clear(); 
    }
}

void Executor :: worker_loop()
{
    while (running)
    {
        function<void()> job ; 
        {
            unique_lock<mutex> lock(mtx); 
            cv.wait(lock, [this]()
            {
                return !tasks.empty() or !running; 
            }) ;
            if (!running and tasks.empty())
            return ; 
            if (tasks.empty())
            {
                job = move(tasks.front());
                tasks.pop(); 
            }
        }

        if (job)
        {
            try 
            {
                job(); 
            } catch(const exception &e)
            {
                cout <<"Executor - task exception" << e.what(); 
            } catch(...){
                cout <<"Executor - task unknown exception" << endl; 
            }
        }
    }
}
