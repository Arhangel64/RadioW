#include "blob.h"
#include <arpa/inet.h>
#include <string>

W::Blob::Blob():
    W::Object(),
    hasData(false),
    dataSize(0),
    data(0)
{
}

W::Blob::Blob(uint32_t size, char* data):
    W::Object(),
    hasData(true),
    dataSize(size),
    data(new char[size])
{
}

W::Blob::Blob(const W::Blob& original):
    W::Object(),
    hasData(original.data),
    dataSize(original.dataSize),
    data(0)
{
    if (hasData) {
        data = new char[dataSize];
        std::copy(original.data, original.data + dataSize, data);
    }
}

W::Blob::~Blob()
{
    if (hasData) {
        delete [] data;
    }
}

W::Blob & W::Blob::operator=(const W::Blob& original)
{
    if (&original != this) 
    {
        if (hasData) {
            delete [] data;
        }
        hasData = original.hasData;
        dataSize = original.dataSize;
        if (hasData) {
            data = new char[dataSize];
            std::copy(original.data, original.data + dataSize, data);
        }
    }
    return *this;
}

W::Object * W::Blob::copy() const
{
    return new W::Blob(*this);
}

W::Object::objectType W::Blob::getType() const
{
    return type;
}

W::Object::size_type W::Blob::size() const
{
    return dataSize;
}

void W::Blob::serialize(W::ByteArray& out) const
{
    pushSize(out);
    for (uint32_t i = 0; i < dataSize; ++i) {
        out.push(data[i]);
    }
}

void W::Blob::deserialize(W::ByteArray& in)
{
    if (hasData) {
        delete [] data;
    }
    
    dataSize = popSize(in);
    if (dataSize > 0) {
        hasData = true;
        data = new char[dataSize];
        for (uint32_t i = 0; i < dataSize; ++i) {
            data[i] = in.pop();
        }
    } else {
        hasData = false;
    }
}

W::Object::StdStr W::Blob::toString() const
{
    return "Blob <" + std::to_string(dataSize) + ">";
}

bool W::Blob::operator==(const W::Blob& other) const
{
    if (dataSize != other.dataSize) {
        return false;
    } else {
        bool equals = true;
        uint64_t i = 0;
        
        while (equals && i < dataSize) {
            equals = data[i] == other.data[i];      //TODO not sure about the c++ syntax if i'm comparing values or addresses this time
            ++i;
        }
        
        return equals;
    }
}

bool W::Blob::operator==(const W::Object& other) const
{
    if (sameType(other)) {
        return operator==(static_cast<const W::Blob&>(other));
    } else {
        return false;
    }
}
