#ifndef UINT64_H
#define UINT64_H

#include <stdint.h>

#include "object.h"

namespace W
{
    class Uint64:
        public Object
    {
    public:
        Uint64();
        explicit Uint64(const uint64_t& original);
        Uint64(const Uint64& original);
        ~Uint64();
        
        Uint64& operator=(const Uint64& original);
        
        StdStr toString() const;
        Object* copy() const;
        size_type size() const;
        objectType getType() const;
        
        bool operator<(const Uint64& other) const;
        bool operator>(const Uint64& other) const;
        bool operator<=(const Uint64& other) const;
        bool operator>=(const Uint64& other) const;
        bool operator==(const Uint64& other) const;
        bool operator!=(const Uint64& other) const;
        
        static const objectType type = uint64;
        
        void serialize(ByteArray& out) const;
        void deserialize(ByteArray& in);
        
    private:
        uint64_t data;
        
    };
}

#endif // UINT64_H
