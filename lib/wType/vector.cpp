#include "vector.h"
#include <string>

W::Vector::Vector():
    Object(),
    data(new Vec())
{

}

W::Vector::Vector(const W::Vector& original):
    Object(),
    data(new Vec())
{
    data->reserve(original.data->capacity());
    
    Vec::const_iterator itr = original.data->begin();
    Vec::const_iterator end = original.data->end();
    
    for (; itr != end; ++itr)
    {
        data->push_back((*itr)->copy());
    }
}


W::Vector::~Vector()
{
    clear();
    delete data;
}

W::Vector& W::Vector::operator=(const W::Vector& original)
{
    if (&original != this)
    {
        clear();
        Vec::const_iterator itr = original.data->begin();
        Vec::const_iterator end = original.data->end();
    
        for (; itr != end; ++itr)
        {
            data->push_back((*itr)->copy());
        }
    }
    return *this;
}

void W::Vector::clear()
{
    Vec::iterator itr = data->begin();
    Vec::iterator end = data->end();
    
    for (; itr != end; ++itr)
    {
        delete (*itr);
    }
    
    data->clear();
}

W::Object::StdStr W::Vector::toString() const
{
    StdStr result;
    
    Vec::const_iterator itr;
    Vec::const_iterator beg = data->begin();
    Vec::const_iterator end = data->end();
    
    result += "[";
    for (itr = beg; itr != end; ++itr)
    {
        if (itr != beg)
        {
            result += ", ";
        }
        result += (*itr)->toString();
    }
    result += "]";
    
    return result;
}

W::Object* W::Vector::copy() const
{
    return new Vector(*this);
}

W::Object::size_type W::Vector::length() const
{
    return data->size();
}

W::Object::size_type W::Vector::size() const
{
    size_type size = 4;
    
    Vec::const_iterator itr = data->begin();
    Vec::const_iterator end = data->end();
    for (; itr != end; ++itr)
    {
        size += (*itr)->size() + 1;
    }
    
    return size;
}

W::Object::objectType W::Vector::getType() const
{
    return Vector::type;
}

void W::Vector::serialize(W::ByteArray& out) const
{
    out.push32(length());
    
    Vec::const_iterator itr = data->begin();
    Vec::const_iterator end = data->end();
    for (; itr != end; ++itr)
    {
        out.push8((*itr)->getType());
        (*itr)->serialize(out);
    }
}

void W::Vector::deserialize(W::ByteArray& in)
{
    data->clear();
    
    size_type length = in.pop32();
    
    for (size_type i = 0; i != length; ++i)
    {
        Object* value = Object::fromByteArray(in);
        data->push_back(value);
    }
}

void W::Vector::push(const W::Object& value)
{
    data->push_back(value.copy());
}

void W::Vector::push(W::Object* value)
{
    data->push_back(value);
}

const W::Object& W::Vector::at(uint64_t index) const
{
    if (index >= length()) {
        throw NoElement(index);
    }
    return *(data->at(index));
}

W::Vector::NoElement::NoElement(uint64_t index):
    Exception(),
    key(index)
{
    
}
std::string W::Vector::NoElement::getMessage() const
{
    std::string msg("No element has been found by index: ");
    msg += std::to_string(key);
    
    return msg;
}


bool W::Vector::operator==(const W::Object& other) const
{
    if (sameType(other)) {
        return operator==(static_cast<const W::Vector&>(other));
    } else {
        return false;
    }
}

bool W::Vector::operator==(const W::Vector& other) const
{
    bool equals = data->size() == other.data->size();
    int i = 0;
    while (equals && i != data->size()) {
        equals = data->at(i) == other.data->at(i);
        ++i;
    }
    
    return equals;
}
