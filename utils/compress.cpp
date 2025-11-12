#include "compress.h"

namespace utils {

vector<uint8_t> Compress::rle_encode(const vector<uint8_t>& data) 
{
    vector<uint8_t> out;
    if (data.empty()) return out;
    uint8_t prev = data[0];
    uint8_t count = 1;
    for (size_t i = 1; i < data.size(); ++i) 
    {
        if (data[i] == prev && count < 255) 
        {
            count++;
        } 
        else 
        {
            out.push_back(count);
            out.push_back(prev);
            prev = data[i];
            count = 1;
        }
    }
    out.push_back(count);
    out.push_back(prev);
    return out;
}

vector<uint8_t> Compress::rle_decode(const vector<uint8_t>& data) 
{
    vector<uint8_t> out;
    for (size_t i = 0; i + 1 < data.size(); i += 2) 
    {
        uint8_t count = data[i];
        uint8_t value = data[i + 1];
        out.insert(out.end(), count, value);
    }
    return out;
}

string Compress::rle_encode_str(const string& s)
{
    vector<uint8_t> data(s.begin(), s.end());
    auto enc = rle_encode(data);
    return string(enc.begin(), enc.end());
}

string Compress::rle_decode_str(const string& s) 
{
    vector<uint8_t> data(s.begin(), s.end());
    auto dec = rle_decode(data);
    return string(dec.begin(), dec.end());
}

} // namespace utils
