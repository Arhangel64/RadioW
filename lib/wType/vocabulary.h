#ifndef VOCABULARY_H
#define VOCABULARY_H

#include "object.h"
#include "string.h"

#include <utils/exception.h>

#include <map>

namespace W
{
    class Vocabulary:
        public Object
    {
        typedef std::map<String, Object*> Map;
        
    public:
        Vocabulary();
        Vocabulary(const Vocabulary& original);
        ~Vocabulary();
        
        Vocabulary& operator=(const Vocabulary& original);
        
        StdStr toString() const;
        Object* copy() const;
        size_type size() const;
        objectType getType() const;
        
        void clear();
        void insert(const String::u32string& key, const Object& value);
        void insert(const String& key, const Object& value);
        const Object& at(const String::u32string& key) const;
        const Object& at(const String& key) const;
        
        static const objectType type = vocabulary;
        
        void serialize(ByteArray& out) const;
        void deserialize(ByteArray& in);
        
        class NoElement:
            Utils::Exception
        {
        public:
            NoElement(const String& p_key);
            
            std::string getMessage() const;
            
        private:
            String key;
        };
        
    private:
        Map *data;
        
    };
}
#endif // VOCABULARY_H
