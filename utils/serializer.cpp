#include "serializer.h"
using namespace std; 

#include <sstream>
#include <iomanip>

#include <stdexcept>
#include <string> 

namespace utils 
{
    string Serializer :: to_string(const unordered_map<string, string> &kv)
    {
        ostringstream oss; 
        for (const auto &[k,v] : kv)
        oss << k << "=" << v << "\n"; 
        return oss.str(); 
    }

    unordered_map<string, string> Serializer :: from_string(const string &data)
    {
        unordered_map<string, string> kv; 
        istringstream iss(data); 
        string line; 
        while (getline(iss, line)){
            size_t eq = line.find("="); 
            if (eq != string :: npos)
            kv[line.substr(0, eq)] = line.substr(eq+ 1); 
        }
        return kv; 
    }

    string Serializer :: to_hex(const vector<uint8_t> &data)
    {
        ostringstream oss;
        for (uint8_t byte : data)
        oss << hex << setw(2) << setfill('0') << (int)byte;
        return oss.str(); 
    }

    
}