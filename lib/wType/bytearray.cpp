#include "bytearray.h"

#include "object.h"

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
    push(object.getType());
    object.serialize(*this);
    
    return *this;
}

W::ByteArray& W::ByteArray::operator>>(W::Object& object)
{
    object.deserialize(*this);
    
    return *this;
}

void W::ByteArray::push(uint8_t byte)
{
    data->push_back(byte);
}

uint8_t W::ByteArray::pop()
{
    uint8_t byte = data->front();
    data->pop_front();
    return byte;
}

