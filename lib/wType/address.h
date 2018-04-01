#ifndef ADDRESS_H
#define ADDRESS_H

#include "object.h"
#include "string.h"
#include "uint64.h"

#include <list>
#include <initializer_list>

namespace W
{
    class Address:
        public Object
    {
        typedef std::list<String> List;
        typedef std::initializer_list<String::u16string> InitList;
        
    public:
        Address();
        Address(const InitList& list);
        Address(const Address& original);
        ~Address();
        
        Address& operator=(const Address& original);
        
        StdStr toString() const override;
        Object* copy() const override;
        size_type length() const override;
        size_type size() const override;
        
        objectType getType() const override;
        
        void serialize(ByteArray& out) const override;
        void deserialize(ByteArray& in) override;
        
        bool operator==(const Object & other) const override;
        
        bool operator<(const Address& other) const;
        bool operator>(const Address& other) const;
        bool operator<=(const Address& other) const;
        bool operator>=(const Address& other) const;
        bool operator==(const Address& other) const;
        bool operator!=(const Address& other) const;
        
        bool begins(const Address& other) const;
        bool ends(const Address& other) const;
        bool contains(const Address& other, int position) const;
        
        Address& operator+=(const Address& other);
        Address& operator+=(const String& other);
        Address& operator+=(const Uint64& other);
        Address& operator+=(const String::u16string& other);
        
        Address operator>>(size_type count) const;
        Address operator<<(size_type count) const;
        Address operator+(const Address& other) const;
        Address operator+(const Uint64& other) const;
        
        static const objectType type = address;
        
        const String& front() const;
        const String& back() const;
        
    private:
        List *data;
    };
}

#endif // ADDRESS_H
