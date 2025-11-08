#include <iostream>
#include <string> 
#include <mutex> 

#include <queue> 
#include <optional> 
#include <cmath> 

using namespace std; 

struct ControlMessage
{
    string command ; 
    string source; 
    string target; 
    double timestamp; 
}; 

class ControlChannel 
{
    private: 
    mutex mtx; 
    queue<ControlMessage> messageQueue; 

    public: 
    void sendCommand(const ControlMessage &msg){
        lock_guard<mutex> lock(mtx); 
        messageQueue.push(msg); 
        cout << "Control Channel" << msg.command << endl; 
    }

    optional<ControlMessage> receiveCommand()
    {
        lock_guard <mutex> lock(mtx); 
        if (messageQueue.empty())
        return nullopt; 

        auto msg = messageQueue.front(); 
        messageQueue.pop(); 
        cout << "Command receive:" << msg.target << endl; 
        return msg; 
    }
}; 
