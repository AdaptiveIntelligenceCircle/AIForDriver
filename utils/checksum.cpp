#include "checksum.h"

using namespace std; 

namespace utils
{
    static const uint32_t crc_table[256] =
    {
        0x00000000L, 0x77073096L, 0xEE0E612CL, 0x990951BAL,
    // shortened for brevity — you can generate full table if needed
    }; 

    uint32_t Checksum :: compute(const vector<uint8_t> &data, CheckSumType type)
    {
        return compute(string(data.begin(), data.end()), type); 
    }

    uint32_t Checksum :: compute(const string &data, CheckSumType type)
    {
        const uint8_t *ptr = reinterpret_cast<const uint8_t*> (data.data()); 
        switch(type)
        {
            case CheckSumType :: CRC32 : return crc32(ptr, data.size()); 
            case CheckSumType :: ADLER32 : return adler32(ptr, data.size()); 
            case CheckSumType :: XORB : return xor8(ptr, data.size()); 
        }
        return 0; 
    }

    uint32_t Checksum :: crc32(const uint8_t *data, size_t len)
    {
        uint32_t crc = 0xFFFFFFF; 
        for (size_t i = 0; i< len ; ++i){
            crc ^= data[i];
            for (int j = 0 ; j < 8 ; ++j){
                crc = (crc >> 1) ^(0xEDB88320 & (-(int)(crc & 1))) ; 
            } 
        }
        return ~crc; 
    }

    uint32_t Checksum :: adler32(const uint8_t *data, size_t len)
    {
        const uint32_t MOD_ADLER = 65521; 
        uint32_t a = 1, b = 0; 
        for (size_t i = 0 ; i < len ; ++i){
            a = (a + data[i]) % MOD_ADLER; 
            b = (b + a) % MOD_ADLER; 
        }
        return (b << 16) | a; 
    }

    uint32_t Checksum :: xor8(const uint8_t *data, size_t len)
    {
        uint8_t result = 0; 
        for (size_t i = 0 ; i < len ; ++i)
        result ^= data[i]; 
        return result; 
    }
} // namespace utils.. 

