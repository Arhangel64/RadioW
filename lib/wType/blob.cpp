#include "blob.h"
#include <arpa/inet.h>
#include <string>

W::Blob::Blob():
    W::Object(),
    hasData(false),
    dataSize(0),
    data(0),
    qDataView()
{
}

W::Blob::Blob(uint32_t size, char* p_data):
    W::Object(),
    hasData(true),
    dataSize(size),
    data(p_data),
    qDataView(QByteArray::fromRawData(p_data, size))
{
}

W::Blob::Blob(const W::Blob& original):
    W::Object(),
    hasData(original.data),
    dataSize(original.dataSize),
    data(0),
    qDataView()
{
    if (hasData) {
        data = new char[dataSize];
        std::copy(original.data, original.data + dataSize, data);
        qDataView = QByteArray::fromRawData(data, dataSize);
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
            qDataView = QByteArray();
        }
        hasData = original.hasData;
        dataSize = original.dataSize;
        if (hasData) {
            data = new char[dataSize];
            std::copy(original.data, original.data + dataSize, data);
            qDataView = QByteArray::fromRawData(data, dataSize);
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

W::Object::size_type W::Blob::length() const
{
    return dataSize;
}

void W::Blob::serialize(W::ByteArray& out) const
{
    out.push32(length());
    for (uint32_t i = 0; i < dataSize; ++i) {
        out.push8(data[i]);
    }
}

void W::Blob::deserialize(W::ByteArray& in)
{
    if (hasData) {
        delete [] data;
        qDataView = QByteArray();
    }
    
    dataSize = in.pop32();
    if (dataSize > 0) {
        hasData = true;
        data = new char[dataSize];
        for (uint32_t i = 0; i < dataSize; ++i) {
            data[i] = in.pop8();
        }
        qDataView = QByteArray::fromRawData(data, dataSize);
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

W::Object::size_type W::Blob::size() const
{
    return dataSize + 4;
}

const QByteArray & W::Blob::byteArray() const
{
    return qDataView;
}

