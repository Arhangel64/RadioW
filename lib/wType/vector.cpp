#include "vector.h"
#include <string>

W::Vector::Vector():
    Object(),
    data(new Vec())
{

}

W::Vector::Vector(const W::Vector& original):
    Object(),
    data(new Vec(original.data->size()))
{
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

W::Object::size_type W::Vector::size() const
{
    return data->size();
}

W::Object::objectType W::Vector::getType() const
{
    return Vector::type;
}

void W::Vector::serialize(W::ByteArray& out) const
{
    pushSize(out);
    
    Vec::const_iterator itr = data->begin();
    Vec::const_iterator end = data->end();
    for (; itr != end; ++itr)
    {
        out.push((*itr)->getType());
        (*itr)->serialize(out);
    }
}

void W::Vector::deserialize(W::ByteArray& in)
{
    data->clear();
    
    size_type length = popSize(in);
    
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


const W::Object& W::Vector::at(uint32_t index) const
{
    if (index >= size()) {
        throw NoElement(index);
    }
    return *(data->at(index));
}

W::Vector::NoElement::NoElement(uint32_t index):
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
