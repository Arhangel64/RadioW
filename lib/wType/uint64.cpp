#include "uint64.h"
#include <string>

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
    
    return *this;
}

W::Object::StdStr W::Uint64::toString() const
{
    return std::to_string(data);
}

W::Object* W::Uint64::copy() const
{
    return new W::Uint64(*this);
}

W::Object::size_type W::Uint64::length() const
{
    return 1;
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
    out.push64(data);
}

void W::Uint64::deserialize(W::ByteArray& in)
{
    data = in.pop64();
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
