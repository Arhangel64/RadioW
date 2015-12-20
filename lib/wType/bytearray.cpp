#include "bytearray.h"

#include "object.h"
#include <arpa/inet.h>

W::ByteArray::ByteArray():
    data(new Container())
{
    
}

W::ByteArray::ByteArray(const W::ByteArray& original):
    data(new Container(*(original.data)))
{

}


W::ByteArray::~ByteArray()
{
    delete data;
}

W::ByteArray& W::ByteArray::operator=(const W::ByteArray& original)
{
    if (&original != this)
    {
        delete data;
        data = new Container(*(original.data));
    }
    return *this;
}

W::ByteArray::size_type W::ByteArray::size() const
{
    return data->size();
}

W::ByteArray& W::ByteArray::operator<<(const W::Object& object)
{
    operator<<(object.getType());
    object.serialize(*this);
    
    return *this;
}

W::ByteArray& W::ByteArray::operator>>(W::Object& object)
{
    object.deserialize(*this);
    
    return *this;
}

W::ByteArray& W::ByteArray::operator<<(const uint32_t& item)
{
    uint32_t converted = htonl(item);
    for (uint8_t i(0); i < 4; ++i)
    {
        data->push_back(((uint8_t*)&converted)[i]);
    }
    return *this;
}

W::ByteArray& W::ByteArray::operator>>(uint32_t& item)
{
    item = pop();
    
    return *this;
}

uint32_t W::ByteArray::pop()
{
    uint8_t source[4] = {0,0,0,0};
    
    for (uint8_t i(0); i < 4; ++i)
    {
        source[i] = data->front();
        data->pop_front();
    }
    return ntohl(*((uint32_t*) source));
}

