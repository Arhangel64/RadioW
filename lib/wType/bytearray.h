#ifndef BYTEARRAY_H
#define BYTEARRAY_H

#include <vector>
#include <stdint.h>

namespace W 
{
    class ByteArray
    {
        friend class Socket;
        typedef std::vector<char> Container;
        
    public:
        typedef uint32_t size_type;
        
        ByteArray(size_type size);
        ByteArray(const ByteArray& original);
        ~ByteArray();
        
        ByteArray& operator=(const ByteArray& original);
        
        void push8(uint8_t integer);
        void push16(uint16_t integer);
        void push32(uint32_t integer);
        void push64(uint64_t integer);
        
        uint8_t pop8();
        uint16_t pop16();
        uint32_t pop32();
        uint64_t pop64();
        
        size_type size() const;
        size_type maxSize() const;
        bool filled() const;
        size_type fill(const char* data, size_type size, size_type shift = 0);
        char* getData();
        
    private:
        Container *data;
        size_type shiftBegin;
        size_type shiftEnd;
        bool referenceMode;
        
    };
}


#endif // BYTEARRAY_H
