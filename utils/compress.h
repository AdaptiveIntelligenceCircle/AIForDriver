#pragma once 
#include <string>

#include <vector> 
#include <cstdint>

using namespace std; 

namespace utils
{
    class Compress{
        public: 
        // Simple RLE encode/decode for small payloads,, 
        static vector<uint8_t> rle_encode(const vector<uint8_t> &data); 
        static vector<uint8_t> rle_decode(const vector<uint8_t> &data); 

        // Convienience for string.. 
        static string rle_encode_str(const string &s); 
        static string rle_decode_str(const string &s); 
    };
}