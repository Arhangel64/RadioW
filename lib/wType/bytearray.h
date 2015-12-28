#ifndef BYTEARRAY_H
#define BYTEARRAY_H

#include <list>
#include <stdint.h>

namespace W 
{
    class Object;
    
    class ByteArray
    {
        friend class Socket;
        typedef std::list<uint8_t> Container;
        
    public:
        typedef uint32_t size_type;
        
        ByteArray();
        ByteArray(const ByteArray& original);
        ~ByteArray();
        
        ByteArray& operator=(const ByteArray& original);
        
        ByteArray& operator<<(const Object& object);
        ByteArray& operator>>(Object& object);
        
        ByteArray& operator<<(const uint32_t& item);
        ByteArray& operator>>(uint32_t& item);
        
        uint32_t pop();
        
        size_type size() const;
        
    private:
        Container *data;
        
    };
}


#endif // BYTEARRAY_H
