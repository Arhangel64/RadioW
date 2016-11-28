#ifndef VECTOR_H
#define VECTOR_H

#include "object.h"
#include <utils/exception.h>
#include <vector>
#include <stdint.h>

namespace W
{
    class Vector:
        public Object
    {
        typedef std::vector<Object*> Vec;
        
    public:
        Vector();
        Vector(const Vector& original);
        ~Vector();
        
        Vector& operator=(const Vector& original);
        
        StdStr toString() const;
        Object* copy() const;
        size_type size() const;
        objectType getType() const;
        
        void clear();
        void push(const Object& value);
        void push(Object* value);
        const Object& at(uint32_t index) const;
        
        static const objectType type = vector;
        
        void serialize(ByteArray& out) const;
        void deserialize(ByteArray& in);
        
        class NoElement:
            Utils::Exception
        {
        public:
            NoElement(uint32_t index);
            
            std::string getMessage() const;
            
        private:
            uint32_t key;
        };
        
    private:
        Vec *data;
        
    };
}

#endif // VECTOR_H
