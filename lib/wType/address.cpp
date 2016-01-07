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

W::Object::size_type W::Address::size() const
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
    out << size();
    
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
    
    size_type length;
    in >> length;
    
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

W::Address& W::Address::operator+=(const W::Address& other)
{
    data->insert(data->end(), other.data->begin(), other.data->end());
    return *this;
}

W::Address& W::Address::operator+=(const W::String& other)
{
    data->push_back(other);
}

W::Address& W::Address::operator+=(const W::String::u32string& other)
{
    String hop(other);
    operator+=(hop);
}

const W::String& W::Address::front() const
{
    return data->front();
}

const W::String& W::Address::back() const
{
    return data->back();
}
