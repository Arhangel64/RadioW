#include "uint64.h"
#include <string>
#include <arpa/inet.h>

W::Uint64::Uint64():
    Object(),
    data(0)
{

}

W::Uint64::Uint64(const uint64_t& original):
    Object(),
    data(original)
{

}

W::Uint64::Uint64(const W::Uint64& original):
    Object(),
    data(original.data)
{

}

W::Uint64::~Uint64()
{

}

W::Uint64& W::Uint64::operator=(const W::Uint64& original)
{
    data = original.data;
}

W::Object::StdStr W::Uint64::toString() const
{
    return std::to_string(data);
}

W::Object* W::Uint64::copy() const
{
    return new W::Uint64(*this);
}

W::Object::size_type W::Uint64::size() const
{
    return 8;
}

W::Object::objectType W::Uint64::getType() const
{
    return type;
}

bool W::Uint64::operator<(const W::Uint64& other) const
{
    return data < other.data;
}

bool W::Uint64::operator>(const W::Uint64& other) const
{
    return data > other.data;
}

bool W::Uint64::operator==(const W::Uint64& other) const
{
    return data == other.data;
}

bool W::Uint64::operator!=(const W::Uint64& other) const
{
    return data != other.data;
}

bool W::Uint64::operator<=(const W::Uint64& other) const
{
    return data <= other.data;
}

bool W::Uint64::operator>=(const W::Uint64& other) const
{
    return data >= other.data;
}

void W::Uint64::serialize(W::ByteArray& out) const
{
    uint32_t h = data >> 32;
    uint32_t l = data & 0x00000000FFFFFFFF;
    
    uint32_t convertedh = htonl(h);
    for (uint8_t i(0); i < 4; ++i)
    {
        out.push(((uint8_t*)&convertedh)[i]);
    }
    
    uint32_t convertedl = htonl(l);
    for (uint8_t i(0); i < 4; ++i)
    {
        out.push(((uint8_t*)&convertedl)[i]);
    }
}

void W::Uint64::deserialize(W::ByteArray& in)
{
    uint8_t srch[4] = {0,0,0,0};
    uint8_t srcl[4] = {0,0,0,0};
    
    for (uint8_t i(0); i < 4; ++i)
    {
        srch[i] = in.pop();
    }
    
    for (uint8_t i(0); i < 4; ++i)
    {
        srcl[i] = in.pop();
    }
    
    uint64_t h = ntohl(*((uint32_t*) srch));
    uint32_t l = ntohl(*((uint32_t*) srcl));
    
    data = (h << 32) | l;
}

W::Uint64::operator uint64_t() const
{
    return data;
}

bool W::Uint64::operator==(const W::Object& other) const
{
    if (sameType(other)) {
        return operator==(static_cast<const W::Uint64&>(other));
    } else {
        return false;
    }
}
