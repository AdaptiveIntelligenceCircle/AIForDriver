#pragma once 
#include <string> 

#include <vector> 
#include <cstdint>

#include <unordered_map>
using namespace std; 

namespace utils
{
    class Serializer
    {
        public: 
        // Serializer key - value map to string.. 
        static string to_string(const unordered_map<string, string>&kv); 

        // Deserialize string to key - value map..// 
        static unordered_map<string, string> from_string(const string &data ); 

        // Convert binary vector to hex string..//
        static string to_hex(const vector<uint8_t>&data); 

        // convert hex string to binary.. // 
        static vector<uint8_t> from_hex(const string &hex); 
    }; 
}