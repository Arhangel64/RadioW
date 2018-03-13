#ifndef BLOB_H
#define BLOB_H

#include "object.h"
#include <QtCore/QByteArray>

namespace W
{
    class Blob: public Object
    {
    public:
        Blob();
        Blob(uint32_t size, char* p_data);
        Blob(const Blob& original);
        ~Blob();
        
        Blob& operator=(const Blob& original);
        
        StdStr toString() const override;
        Object* copy() const override;
        size_type size() const override;
        
        objectType getType() const override;
        
        bool operator==(const W::Object & other) const override;
        
        void serialize(ByteArray& out) const override;
        void deserialize(ByteArray& in) override;
        
        bool operator==(const Blob& other) const;
        
        static const objectType type = blob;
        
        const QByteArray& byteArray() const;
        
    protected:
        bool hasData;
        uint32_t dataSize;
        char* data;
        QByteArray qDataView;
        
    };
}

#endif // BLOB_H
