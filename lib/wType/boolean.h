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
        
        StdStr toString() const override;
        Object* copy() const override;
        size_type size() const override;
        objectType getType() const override;
        bool operator==(const W::Object & other) const override;
        
        bool operator<(const Boolean& other) const;
        bool operator>(const Boolean& other) const;
        bool operator<=(const Boolean& other) const;
        bool operator>=(const Boolean& other) const;
        bool operator==(const Boolean& other) const;
        bool operator!=(const Boolean& other) const;
        
        static const objectType type = boolean;
        
        void serialize(ByteArray& out) const override;
        void deserialize(ByteArray& in) override;
        
        operator bool() const;
        
    private:
        bool data;
        
    };
}

#endif // BOOLEAN_H
