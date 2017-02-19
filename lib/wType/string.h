#ifndef W_STRING_H
#define W_STRING_H

#include "object.h"

namespace W{

    class String : public Object
    {
    public:
        typedef std::u16string u16string;
        
        String();
        explicit String(const u16string& p_data);
        explicit String(const char16_t* p_data);
        explicit String(const StdStr p_data);
        String(const String& original);
        
        ~String();
        
        String& operator=(const String& original);
        
        StdStr toString() const;
        Object* copy() const;
        size_type size() const;
        objectType getType() const;
        
        bool operator<(const String& other) const;
        bool operator>(const String& other) const;
        bool operator<=(const String& other) const;
        bool operator>=(const String& other) const;
        bool operator==(const String& other) const;
        bool operator!=(const String& other) const;
        
        bool operator==(const char16_t* other) const;
        bool operator!=(const char16_t* other) const;
        
        static const objectType type = string;
        
        operator u16string() const;
        uint64_t toUint64() const;
        
        void serialize(ByteArray& out) const;
        void deserialize(ByteArray& in);
        
        String& operator+=(int);
        String& operator+=(const String& other);
        String operator+(const String& other) const;
        
        size_type findLastOf(const W::String& str) const;
        size_type findFirstOf(const W::String& str) const;
        
        String substr(size_type start, size_type length = u16string::npos) const;
        
    private:
        u16string* data;
        
    };

}

#endif // W_STRING_H
