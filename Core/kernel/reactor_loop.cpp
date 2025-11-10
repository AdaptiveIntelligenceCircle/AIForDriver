#include "reactor_loop.h"
using namespace std; 

#include <algorithm>
#include <iostream> 

ReactorLoop :: ReactorLoop() : running (false), next_id(1){}
ReactorLoop :: ~ReactorLoop()
{
    stop(); 
}

int ReactorLoop :: register_periodic(Callback cb, uint64_t period_ms, uint64_t intial_delay_ms)
{
    lock_guard<mutex> lock(mtx);
    Item it; 
    it.id = next_id ++; 
    it.cb = move(cb); 
    it.period = chrono :: milliseconds(period_ms); 
    it.next = chrono :: steady_clock:: now() + chrono :: milliseconds(intial_delay_ms); 
    it.cancelled = false; 
    items.push_back(move(it)); 
    return next_id - 1; 
}

void ReactorLoop :: unregister(int id)
{
    lock_guard<mutex> lock(mtx); 
    for (auto &it : items)
    {
        if (it.id == id)
        {
            it.cancelled = true; 
            break; 
        }
    }
    // lazy removal happnes in loop_fn

}

void ReactorLoop :: start()
{
    bool exp = false; 
    if (!running.compare_exchange_strong(exp, true)) 
    return ; 
    loop_thread = thread([this]()
    {
        loop_thread_fn(); 
    }); 
}

void ReactorLoop :: stop()
{
    bool exp = true; 
    if (running.compare_exchange_strong(exp, false)){
        if (loop_thread.joinable()) 
        loop_thread.join(); 
        lock_guard<mutex> lock(mtx); 
        items.clear(); 
    }
}

bool ReactorLoop :: is_running() const 
{
    return running.load(); 
}

void ReactorLoop :: loop_thread_fn()
{
    using namespace chrono; 
    while (running)
    {
        vector<Item> due; 
        {
            lock_guard<mutex> lock(mtx) ; 
            auto now = steady_clock :: now(); 
            for (auto &it : items)
            {
                if (it.cancelled) continue ; 
                if (it.next <= now){
                    due.push_back(it);
                    if (it.period.count() == 0)
                    {
                        it.cancelled = true; 
                    }
                    else 
                    {
                        it.next = now + it.period;
                    }
                }
            }

            // Remove cancelled entries..// 
            items.erase(remove_if(items.begin(), items.end(), [](const Item& a){ 
                return a.cancelled; }), items.end());
        }

        for (auto &it : due)
        {
            try 
            {
                it.cb(); 
            }catch(const exception &e)
            {
                cout << "ReactionLoop callback exception:" << e.what() << endl; 
            }catch (...)
            {
                cout <<"Reactor Loop - callback unknown exception" << endl; 
            }
        }
        this_thread :: sleep_for(milliseconds(5)); 
    }
}

