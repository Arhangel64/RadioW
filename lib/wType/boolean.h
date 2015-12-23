#ifndef BOOLEAN_H
#define BOOLEAN_H

#include "object.h"

namespace W
{
    class Boolean:
        public Object
    {
    public:
        Boolean();
        explicit Boolean(bool value);
        Boolean(const Boolean& original);
        
        ~Boolean();
        
        Boolean& operator=(const Boolean& original);
        Boolean& operator=(bool original);
        
        StdStr toString() const;
        Object* copy() const;
        size_type size() const;
        objectType getType() const;
        
        bool operator<(const Boolean& other) const;
        bool operator>(const Boolean& other) const;
        bool operator<=(const Boolean& other) const;
        bool operator>=(const Boolean& other) const;
        bool operator==(const Boolean& other) const;
        bool operator!=(const Boolean& other) const;
        
        static const objectType type = boolean;
        
        void serialize(ByteArray& out) const;
        void deserialize(ByteArray& in);
        
        operator bool() const;
        
    private:
        bool data;
        
    };
}

#endif // BOOLEAN_H
