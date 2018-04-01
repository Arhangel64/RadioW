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
        
        StdStr toString() const override;
        Object* copy() const override;
        size_type length() const override;
        size_type size() const override;
        objectType getType() const override;
        
        bool operator==(const W::Object & other) const override;
        bool operator==(const W::Vector & other) const;
        
        void clear();
        void push(const Object& value);
        void push(Object* value);
        const Object& at(uint64_t index) const;
        
        static const objectType type = vector;
        
        void serialize(ByteArray& out) const override;
        void deserialize(ByteArray& in) override;
        
        class NoElement:
            Utils::Exception
        {
        public:
            NoElement(uint64_t index);
            
            std::string getMessage() const;
            
        private:
            uint64_t key;
        };
        
    private:
        Vec *data;
        
    };
}

#endif // VECTOR_H
