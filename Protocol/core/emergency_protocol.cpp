#include <iostream> 
#include <string> 
using namespace std; 

class EmergencyProtocol
{
    private: 
    bool emergentyActive = false; 

    public: 
    void trigger(const string &reason)
    {
        emergentyActive = true; 
        cout <<"EmergencyProtocol - Emergency triggered:" << reason << endl; 
    }

    void reset()
    {
        emergentyActive = false; 
        cout <<"Emergency Protocol - Rest emergency state" << endl; 
    }

    bool isActive() const 
    {
        return emergentyActive; 
    }
}; 
