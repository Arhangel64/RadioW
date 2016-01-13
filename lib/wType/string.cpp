#include "string.h"

#include <codecvt>
#include <locale>

W::String::String():
    Object(),
    data(new u32string())
{

}

W::String::String(const u32string& p_data):
    Object(),
    data(new u32string(p_data))
{

}

W::String::String(const char32_t* p_data):
    Object(),
    data(new u32string(p_data))
{

}


W::String::String(const W::String& original):
    Object(),
    data(new u32string(*original.data))
{

}

W::String::~String()
{
    delete data;
}

W::String& W::String::operator=(const W::String& original)
{
    if (&original != this) 
    {
        delete data;
        data = new u32string(*(original.data));
    }
    return *this;
}


W::Object* W::String::copy() const
{
    return new String(*this);
}

W::Object::StdStr W::String::toString() const
{
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convertor;
    StdStr result = convertor.to_bytes(*data);
    
    return result;
}

W::Object::size_type W::String::size() const
{
    return data->size();
}

void W::String::serialize(W::ByteArray& out) const
{
    out << size();
    
    u32string::const_iterator itr = data->begin();
    u32string::const_iterator end = data->end();
    
    for(; itr != end; ++itr) 
    {
        out << *itr;
    }
}

void W::String::deserialize(W::ByteArray& in)
{
    data->clear();
    
    ByteArray::size_type length;
    in >> length;
    
    for (ByteArray::size_type i = 0; i != length; ++i)
    {
        uint32_t tmp;
        in >> tmp;
        (*data) += tmp;
    }
}

W::Object::objectType W::String::getType() const
{
    return String::type;
}

bool W::String::operator<(const W::String& other) const
{
    return (*data) < *(other.data);
}

bool W::String::operator>(const W::String& other) const
{
    return (*data) > *(other.data);
}

bool W::String::operator<=(const W::String& other) const
{
    return (*data) <= *(other.data);
}

bool W::String::operator>=(const W::String& other) const
{
    return (*data) >= *(other.data);
}

bool W::String::operator!=(const W::String& other) const
{
    return (*data) != *(other.data);
}

bool W::String::operator==(const W::String& other) const
{
    return (*data) == *(other.data);
}

bool W::String::operator==(const char32_t* other) const
{
    return *data == other;
}

bool W::String::operator!=(const char32_t* other) const
{
    return *data != other;
}

W::String::operator u32string() const
{
    return *data;
}

