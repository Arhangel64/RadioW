#ifndef WOBJCET_H
#define WOBJCET_H

#include <string>
#include <stdint.h>
#include "bytearray.h"

namespace W {

    class Object
    {
        friend class ByteArray;
    public:
        enum objectType
        {
            string,
            vocabulary,
            uint64,
            address,
            boolean,
            event,
            vector,
            blob
        };
        
        typedef uint64_t size_type;
        typedef std::string StdStr;
    
        Object();
        virtual ~Object();
    
        virtual StdStr toString() const = 0;
        virtual Object* copy() const = 0;
        virtual size_type length() const = 0;                       //object specific size - like amount of subelements or amount of characters
        virtual size_type size() const = 0;                         //object size in bytes
        virtual bool operator==(const Object& other) const = 0; 
        virtual bool operator!=(const Object& other) const;         //TODO may be make it also pure virtual?
        
        virtual objectType getType() const = 0;
        
        virtual void serialize(ByteArray& out) const = 0;
        virtual void deserialize(ByteArray& in) = 0;
        
        static StdStr getTypeName(objectType type);
        
    protected:
        bool sameType(const Object& other) const;
        
    public:
        static Object* fromByteArray(ByteArray& in);
        
    };
}

#endif // WOBJCET_H
