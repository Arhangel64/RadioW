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
            event
        };
        
        typedef uint32_t size_type;
        typedef std::string StdStr;
    
        Object();
        virtual ~Object();
    
        virtual StdStr toString() const = 0;
        virtual Object* copy() const = 0;
        virtual size_type size() const = 0;
        
        virtual objectType getType() const = 0;
        
        virtual void serialize(ByteArray& out) const = 0;
        virtual void deserialize(ByteArray& in) = 0;
        
        void pushSize(ByteArray& out) const;
        size_type popSize(ByteArray& in) const;
        
    public:
        static Object* fromByteArray(ByteArray& in);
        
    };
}

#endif // WOBJCET_H
