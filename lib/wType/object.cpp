#include "object.h"

#include "string.h"
#include "vocabulary.h"
#include "uint64.h"
#include "address.h"
#include "boolean.h"
#include "event.h"
#include "vector.h"
#include "blob.h"
#include <arpa/inet.h>
//#include <stdint.h>

W::Object::Object()
{

}

W::Object::~Object()
{

}

void W::Object::pushSize(W::ByteArray& out) const
{
    uint32_t converted = htonl(size());
    for (uint8_t i(0); i < 4; ++i)
    {
        out.push(((uint8_t*)&converted)[i]);
    }
}

W::Object::size_type W::Object::popSize(W::ByteArray& in) const
{
    uint8_t src[4];
    
    for (uint8_t i(0); i < 4; ++i)
    {
        src[i] = in.pop();
    }
    
    
    
    return ntohl(*((uint32_t*) src));
}

W::Object* W::Object::fromByteArray(ByteArray& in)
{
    uint32_t type = in.pop();
    
    Object *answer;
    
    switch (type)
    {
        case string:
            answer = new String();
            break;
            
        case vocabulary:
            answer = new Vocabulary();
            break;
            
        case uint64:
            answer = new Uint64();
            break;
            
        case address:
            answer = new Address();
            break;
            
        case boolean:
            answer = new Boolean();
            break;
            
        case event:
            answer = new Event();
            break;
            
        case vector:
            answer = new Vector();
            break;
            
        case blob:
            answer = new Blob();
            break;
    }
    
    in >> *answer;
    
    return answer;
}

bool W::Object::sameType(const W::Object& other) const
{
    return getType() == other.getType();
}

bool W::Object::operator!=(const W::Object& other) const
{
    return !operator==(other);
}

W::Object::StdStr W::Object::getTypeName(W::Object::objectType type)
{
    switch (type) {
        case string:
            return "String";
        case vocabulary:
            return "Vocabulary";
            
        case uint64:
            return "Uint64";
            
        case address:
            return "Address";
            
        case boolean:
            return "Boolean";
            
        case event:
            return "Event";
            
        case vector:
            return "Vector";
            
        case blob:
            return "Blob";
    }
}

