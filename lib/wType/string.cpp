#include "string.h"

#include <codecvt>
#include <locale>

W::String::String():
    Object(),
    data(new u16string())
{

}

W::String::String(const u16string& p_data):
    Object(),
    data(new u16string(p_data))
{

}

W::String::String(const char16_t* p_data):
    Object(),
    data(new u16string(p_data))
{

}


W::String::String(const W::String& original):
    Object(),
    data(new u16string(*original.data))
{

}

W::String::String(const StdStr p_data):
    Object(),
    data(0)
{
    std::wstring_convert<std::codecvt_utf8<char16_t>,char16_t> convert;
    std::u16string u16 = convert.from_bytes(p_data);
    
    data = new u16string(u16);
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
        data = new u16string(*(original.data));
    }
    return *this;
}


W::Object* W::String::copy() const
{
    return new String(*this);
}

W::Object::StdStr W::String::toString() const
{
    std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> convertor;
    StdStr result = convertor.to_bytes(*data);
    
    return result;
}

W::Object::size_type W::String::length() const
{
    return data->size();
}

void W::String::serialize(W::ByteArray& out) const
{
    out.push32(length());;
    
    u16string::const_iterator itr = data->begin();
    u16string::const_iterator end = data->end();
    
    for(; itr != end; ++itr) 
    {
        out.push16(*itr);
    }
}

void W::String::deserialize(W::ByteArray& in)
{
    data->clear();
    
    size_type length = in.pop32();
    
    for (size_type i = 0; i != length; ++i)
    {
        data->push_back(in.pop16());
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

bool W::String::operator==(const char16_t* other) const
{
    return *data == other;
}

bool W::String::operator!=(const char16_t* other) const
{
    return *data != other;
}

W::String::operator u16string() const
{
    return *data;
}

W::String & W::String::operator+=(int number)
{
    StdStr str = std::to_string(number);;
    std::wstring_convert<std::codecvt_utf8<char16_t>,char16_t> convert;
    std::u16string u16 = convert.from_bytes(str);
    (*data) += u16;
    
    return *this;
}

W::String W::String::operator+(const W::String& other) const
{
    return W::String((*data) + *(other.data));
}

W::String & W::String::operator+=(const W::String& other)
{
    (*data) += *(other.data);
    return *this;
}

W::String::size_type W::String::findFirstOf(const W::String& str) const
{
    return data->find_first_of(*(str.data));
}

W::String::size_type W::String::findLastOf(const W::String& str) const
{
    return data->find_last_of(*(str.data));
}

W::String W::String::substr(size_type start, size_type length) const
{
    return W::String(data->substr(start, length));
}

uint64_t W::String::toUint64() const
{
    return std::stoull(toString());
}

bool W::String::operator==(const W::Object& other) const
{
    if (sameType(other)) {
        return operator==(static_cast<const W::String&>(other));
    } else {
        return false;
    }
}

W::Object::size_type W::String::size() const
{
    return data->size() * 2 + 4;
}
