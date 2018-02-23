#ifndef BLOB_H
#define BLOB_H

#include "object.h"

namespace W
{
    class Blob: public Object
    {
    public:
        Blob();
        Blob(uint32_t size, char* data);
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
        
    protected:
        bool hasData;
        uint32_t dataSize;
        char* data;
        
    };
}

#endif // BLOB_H
