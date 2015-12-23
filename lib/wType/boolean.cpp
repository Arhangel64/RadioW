#include "boolean.h"

W::Boolean::Boolean():
    Object(),
    data(false)
{

}

W::Boolean::Boolean(bool value):
    Object(),
    data(value)
{

}

W::Boolean::Boolean(const W::Boolean& original):
    Object(),
    data(original.data)
{

}

W::Boolean::~Boolean()
{

}

W::Boolean& W::Boolean::operator=(const W::Boolean& original)
{
    data = original.data;
    return *this;
}

W::Boolean& W::Boolean::operator=(bool original)
{
    data = original;
    return *this;
}

W::Object::StdStr W::Boolean::toString() const
{
    StdStr str;
    if (data)
    {
        str = "true";
    }
    else
    {
        str = "false";
    }
    return str;
}

W::Object::objectType W::Boolean::getType() const
{
    return Boolean::type;
}

W::Object::size_type W::Boolean::size() const
{
    return 1;
}

W::Object* W::Boolean::copy() const
{
    return new Boolean(*this);
}

bool W::Boolean::operator>(const W::Boolean& other) const
{
    return data > other.data;
}

bool W::Boolean::operator<(const W::Boolean& other) const
{
    return data < other.data;
}

bool W::Boolean::operator==(const W::Boolean& other) const
{
    return data == other.data;
}

bool W::Boolean::operator!=(const W::Boolean& other) const
{
    return data != other.data;
}

bool W::Boolean::operator<=(const W::Boolean& other) const
{
    return data <= other.data;
}

bool W::Boolean::operator>=(const W::Boolean& other) const
{
    return data >= other.data;
}

void W::Boolean::serialize(W::ByteArray& out) const
{
    uint32_t val;
    if (data)
    {
        val = 0xFFFFFFFF;
    }
    else
    {
        val = 0;
    }
    
    out << val;
}

void W::Boolean::deserialize(W::ByteArray& in)
{
    uint32_t val = in.pop();
    
    if (val == 0)
    {
        data = false;
    }
    else
    {
        data = true;
    }
}

W::Boolean::operator bool() const
{
    return data;
}
