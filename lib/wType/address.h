#ifndef ADDRESS_H
#define ADDRESS_H

#include "object.h"
#include "string.h"

#include <list>
#include <initializer_list>

namespace W
{
    class Address:
        public Object
    {
        typedef std::list<String> List;
        typedef std::initializer_list<String::u32string> InitList;
        
    public:
        Address();
        Address(const InitList& list);
        Address(const Address& original);
        ~Address();
        
        Address& operator=(const Address& original);
        
        StdStr toString() const;
        Object* copy() const;
        size_type size() const;
        
        objectType getType() const;
        
        void serialize(ByteArray& out) const;
        void deserialize(ByteArray& in);
        
        bool operator<(const Address& other) const;
        bool operator>(const Address& other) const;
        bool operator<=(const Address& other) const;
        bool operator>=(const Address& other) const;
        bool operator==(const Address& other) const;
        bool operator!=(const Address& other) const;
        
        Address& operator+=(const Address& other);
        Address& operator+=(const String& other);
        Address& operator+=(const String::u32string& other);
        
        static const objectType type = address;
        
        const String& front() const;
        const String& back() const;
        
    private:
        List *data;
    };
}

#endif // ADDRESS_H
