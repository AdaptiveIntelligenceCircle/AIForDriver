#include "packet_parser.h"
#include <cstring> 

#include <iostream>
using namespace std; 

namespace utils 
{
    vector<uint8_t> PacketParser::encode(const Packet& p, CheckSumType type) {
    string hdr_str = Serializer::to_string(p.header);
    uint16_t hdr_len = static_cast<uint16_t>(hdr_str.size());

    vector<uint8_t> data;
    data.push_back(hdr_len & 0xFF);
    data.push_back((hdr_len >> 8) & 0xFF);

    data.insert(data.end(), hdr_str.begin(), hdr_str.end());
    data.insert(data.end(), p.payload.begin(), p.payload.end());

    uint32_t chk = Checksum::compute(p.payload, type);
    for (int i = 0; i < 4; ++i)
        data.push_back((chk >> (i * 8)) & 0xFF);
    return data;
}

    optional<Packet> PacketParser::decode(const vector<uint8_t>& data) {
    if (data.size() < 6) return nullopt; // minimal header + checksum

    uint16_t hdr_len = data[0] | (data[1] << 8);
    if (hdr_len + 6 > data.size()) return nullopt;

    string hdr_str(reinterpret_cast<const char*>(&data[2]), hdr_len);
    auto hdr = Serializer::from_string(hdr_str);

    size_t payload_len = data.size() - hdr_len - 2 - 4;
    vector<uint8_t> payload(data.begin() + 2 + hdr_len, data.begin() + 2 + hdr_len + payload_len);

    uint32_t chk = 0;
    memcpy(&chk, &data[data.size() - 4], 4);

    return Packet{hdr, payload, chk};
}

bool PacketParser::verify(const Packet& p, CheckSumType type) 
{
    uint32_t calc = Checksum::compute(p.payload, type);
    return calc == p.checksum;
}

}