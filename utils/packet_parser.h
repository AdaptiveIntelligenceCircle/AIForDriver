#pragma once 
#include <string> 

#include <vector> 
#include <unordered_map>

#include <optional> 
#include "serializer.h"

#include "checksum.h"
using namespace std; 

namespace utils
{
    struct Packet
    {
        unordered_map<string, string> header; 
        vector<uint8_t> payload; 
        uint32_t checksum; 
    }; 

    class PacketParser
    {
        public: 
        // encode packet into binary.. '
        static vector<uint8_t> encode(const Packet &p, CheckSumType type = CheckSumType :: CRC32); 

        // decode binary back to packet.. 
        static optional<Packet> decode(const vector<uint8_t> &data); 

        // Verify checksum intergrity..
        static bool verify(const Packet &p, CheckSumType type = CheckSumType :: CRC32); 
    }; 
}