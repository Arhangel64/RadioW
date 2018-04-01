#include "bytearray.h"
#include <arpa/inet.h>

W::ByteArray::ByteArray(size_type size):
    data(new Container(size)),
    shiftBegin(0),
    shiftEnd(0),
    referenceMode(false)
{
    
}

W::ByteArray::ByteArray(const W::ByteArray& original):
    data(new Container(*(original.data))),
    shiftBegin(original.shiftBegin),
    shiftEnd(original.shiftEnd),
    referenceMode(original.referenceMode)
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
        shiftBegin = original.shiftBegin;
        shiftEnd = original.shiftEnd;
        referenceMode = original.referenceMode;
    }
    return *this;
}

W::ByteArray::size_type W::ByteArray::size() const
{
    return shiftEnd - shiftBegin;
}

void W::ByteArray::push8(uint8_t integer)
{
    if (referenceMode) {
        //TODO not sure
    }
    data->at(shiftEnd) = integer;
    ++shiftEnd;
}

void W::ByteArray::push16(uint16_t integer)
{
    uint16_t n16char = htons(integer);
    
    push8(((uint8_t*)&n16char)[0]);
    push8(((uint8_t*)&n16char)[1]);
}


void W::ByteArray::push32(uint32_t integer)
{
    uint32_t converted = htonl(integer);
    for (uint8_t i(0); i < 4; ++i)
    {
        push8(((uint8_t*)&converted)[i]);
    }
}

void W::ByteArray::push64(uint64_t integer)
{
    uint32_t h = integer >> 32;
    uint32_t l = integer & 0x00000000FFFFFFFF;
    
    uint32_t convertedh = htonl(h);
    for (uint8_t i(0); i < 4; ++i)
    {
        push8(((uint8_t*)&convertedh)[i]);
    }
    
    uint32_t convertedl = htonl(l);
    for (uint8_t i(0); i < 4; ++i)
    {
        push8(((uint8_t*)&convertedl)[i]);
    }
}


uint8_t W::ByteArray::pop8()
{
    uint8_t byte = data->at(shiftBegin);
    ++shiftBegin;
    return byte;
}

uint16_t W::ByteArray::pop16()
{
    uint8_t h = pop8();
    uint8_t l = pop8();
    
    uint8_t src[2] = {h, l};
    
    return ntohs(*((uint16_t*) src));
}


uint32_t W::ByteArray::pop32()
{
    uint8_t src[4];             //TODO optimize?
    
    for (uint8_t i(0); i < 4; ++i)
    {
        src[i] = pop8();
    }
    
    return ntohl(*((uint32_t*) src));
}

uint64_t W::ByteArray::pop64()
{
    uint8_t srch[4] = {0,0,0,0};
    uint8_t srcl[4] = {0,0,0,0};
    
    for (uint8_t i(0); i < 4; ++i)
    {
        srch[i] = pop8();
    }
    
    for (uint8_t i(0); i < 4; ++i)
    {
        srcl[i] = pop8();
    }
    
    uint64_t h = ntohl(*((uint32_t*) srch));
    uint32_t l = ntohl(*((uint32_t*) srcl));
    
    return (h << 32) | l;
}


char * W::ByteArray::getData()
{
    return data->data();        //TODO not actually sure about this, may be need to check first about shifts and refence?
}

W::ByteArray::size_type W::ByteArray::maxSize() const
{
    return data->size();
}

bool W::ByteArray::filled() const
{
    return shiftEnd == data->size();
}

W::ByteArray::size_type W::ByteArray::fill(const char* data, W::ByteArray::size_type size, W::ByteArray::size_type shift)
{
    size_type i = shift;
    while (i < size && !filled()) {
        push8(data[i]);
        ++i;
    }
    
    return i;
}
