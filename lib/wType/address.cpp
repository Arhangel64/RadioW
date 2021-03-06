#include "address.h"

W::Address::Address():
    Object(),
    data(new List())
{

}

W::Address::Address(const W::Address::InitList& list):
    Object(),
    data(new List())
{
    InitList::const_iterator itr = list.begin();
    InitList::const_iterator end = list.end();
    
    for (; itr != end; ++itr)
    {
        data->emplace_back(String(*itr));
    }
}

W::Address::Address(const W::Address& original):
    Object(),
    data(new List(*original.data))
{

}

W::Address::~Address()
{
    delete data;
}

W::Address& W::Address::operator=(const W::Address& original)
{
    if (&original != this) 
    {
        data->clear();
        data->insert(data->end(), original.data->begin(), original.data->end());
    }
    return *this;
}

W::Object::size_type W::Address::length() const
{
    return data->size();
}

W::Object::objectType W::Address::getType() const
{
    return type;
}

W::Object::StdStr W::Address::toString() const
{
    StdStr str;
    
    List::const_iterator itr;
    List::const_iterator beg = data->begin();
    List::const_iterator end = data->end();
    
    str += '[';
    for (itr = beg; itr != end; ++itr)
    {
        if (itr != beg)
        {
            str += ", ";
        }
        str += itr->toString();
    }
    str += ']';
    
    return str;
}

W::Object* W::Address::copy() const
{
    return new Address(*this);
}

void W::Address::serialize(W::ByteArray& out) const
{
    out.push32(length());
    
    List::const_iterator itr;
    List::const_iterator beg = data->begin();
    List::const_iterator end = data->end();
    
    for (itr = beg; itr != end; ++itr)
    {
        itr->serialize(out);
    }
}

void W::Address::deserialize(W::ByteArray& in)
{
    data->clear();
    
    size_type length = in.pop32();
    
    for (size_type i = 0; i != length; ++i)
    {
        data->emplace_back(String());
        data->back().deserialize(in);
    }
}

bool W::Address::operator<(const W::Address& other) const
{
    return *data < *other.data;
}

bool W::Address::operator>(const W::Address& other) const
{
    return *data > *other.data;
}

bool W::Address::operator==(const W::Address& other) const
{
    return *data == *other.data;
}

bool W::Address::operator!=(const W::Address& other) const
{
    return *data != *other.data;
}

bool W::Address::operator<=(const W::Address& other) const
{
    return *data <= *other.data;
}

bool W::Address::operator>=(const W::Address& other) const
{
    return *data >= *other.data;
}

bool W::Address::begins(const W::Address& other) const
{
    if (other.length() > length())
    {
        return false;
    }
    
    List::const_iterator itr_o = other.data->begin();
    List::const_iterator end_o = other.data->end();
    
    List::const_iterator itr_i = data->begin();
    
    while (itr_o != end_o)
    {
        if (*itr_o != *itr_i) {
            return false;
        }
        
        ++itr_o;
        ++itr_i;
    }
    
    return true;
}

bool W::Address::ends(const W::Address& other) const
{
    if (other.length() > length())
    {
        return false;
    }
    
    List::const_reverse_iterator itr_o = other.data->rbegin();
    List::const_reverse_iterator end_o = other.data->rend();
    
    List::const_reverse_iterator itr_i = data->rbegin();
    
    while (itr_o != end_o)
    {
        if (*itr_o != *itr_i) {
            return false;
        }
        
        ++itr_o;
        ++itr_i;
    }
    
    return true;
}

bool W::Address::contains(const W::Address& other, int position) const
{
    if (other.length() > length() - position)
    {
        return false;
    }
    
    bool res = true;
    
    List::const_iterator itr_o = other.data->begin();
    List::const_iterator end_o = other.data->end();
    
    List::const_iterator itr_i = data->begin();
    
    for (int i = 0; i < position; ++i) {
        ++itr_i;
    }
    
    while (res == true && itr_o != end_o)
    {
        res = *itr_o == *itr_i;
        
        ++itr_o;
        ++itr_i;
    }
    
    return res;
}

W::Address& W::Address::operator+=(const W::Address& other)
{
    data->insert(data->end(), other.data->begin(), other.data->end());
    return *this;
}

W::Address& W::Address::operator+=(const W::String& other)
{
    data->push_back(other);
    return *this;
}

W::Address& W::Address::operator+=(const W::String::u16string& other)
{
    String hop(other);
    return operator+=(hop);
}

W::Address W::Address::operator>>(W::Object::size_type count) const
{
    W::Address res;
    if (count < length())
    { 
        List::const_iterator itr = data->end();
        for (size_type i = 0; i != count; ++i)
        {
            itr--;
        }
        List::const_iterator beg = data->begin();
        res.data->insert(res.data->end(), beg, itr);
    }
    return res;
}

W::Address W::Address::operator<<(W::Object::size_type count) const
{
    W::Address res;
    if (count < length())
    { 
        List::const_iterator itr = data->begin();
        for (size_type i = 0; i != count; ++i)
        {
            itr++;
        }
        List::const_iterator end = data->end();
        res.data->insert(res.data->end(), itr, end);
    }
    return res;
}

W::Address W::Address::operator+(const W::Address& other) const
{
    W::Address res;
    res += *this;
    res += other;
    
    return res;
}

W::Address & W::Address::operator+=(const Uint64& other)
{
    data->push_back(String(other.toString()));
    
    return *this;
}


W::Address W::Address::operator+(const Uint64& other) const
{
    W::Address res;
    res += *this;
    res += other;
    
    return res;
}


const W::String& W::Address::front() const
{
    return data->front();
}

const W::String& W::Address::back() const
{
    return data->back();
}

bool W::Address::operator==(const W::Object& other) const
{
    if (sameType(other)) {
        return operator==(static_cast<const W::Address&>(other));
    } else {
        return false;
    }
}

W::Object::size_type W::Address::size() const
{
    size_type size = 4;
    List::const_iterator itr = data->begin();
    List::const_iterator end = data->end();
    
    for (; itr != end; ++itr)
    {
        size += itr->size();
    }
    
    return size;
}
