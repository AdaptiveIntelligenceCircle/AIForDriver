#pragma once 
#include <string> 

#include <vector> 
#include <cstdint> 

using namespace std ; 

namespace utils
{
    enum class CheckSumType
    {
        CRC32, 
        ADLER32, 
        XORB 
    };

    class Checksum
    {
        public:
        static uint32_t compute(const vector<uint8_t> &data, CheckSumType type = CheckSumType::CRC32);
        static uint32_t compute(const string &data, CheckSumType type = CheckSumType :: XORB);

        private: 
        static uint32_t crc32(const uint8_t *data, size_t len);
        static uint32_t adler32(const uint8_t * data, size_t len); 
        static uint32_t xor8(const uint8_t *data, size_t len); 
    }; 
} // namespace utils.. // 