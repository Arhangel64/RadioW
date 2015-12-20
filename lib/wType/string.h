#ifndef STRING_H
#define STRING_H

#include "object.h"

namespace W{

    class String : public Object
    {
    public:
        typedef std::u32string u32string;
        
        String();
        explicit String(const u32string& p_data);
        explicit String(const char32_t* p_data);
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
        
        static const objectType type = string;
        
        void serialize(ByteArray& out) const;
        void deserialize(ByteArray& in);
        
    private:
        u32string* data;
        
    };

}

#endif // STRING_H
