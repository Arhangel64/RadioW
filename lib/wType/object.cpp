#include "object.h"

#include "string.h"
#include "vocabulary.h"
#include "uint64.h"
#include "address.h"
//#include <stdint.h>

W::Object::Object()
{

}

W::Object::~Object()
{

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
    }
    
    in >> *answer;
    
    return answer;
}

