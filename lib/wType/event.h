#ifndef EVENT_H
#define EVENT_H

#include "object.h"
#include "address.h"
#include "uint64.h"
#include "boolean.h"

namespace W
{
    class Event:
        public Object
    {
    public:
        Event();
        Event(const Address& p_addr, const Object& p_data, bool p_system = false);
        Event(const Address& p_addr, Object* p_data, bool p_system = false);
        Event(const Event& original);
        ~Event();
        
        Event& operator=(const Event& original);
        
        StdStr toString() const;
        Object* copy() const;
        size_type size() const;
        objectType getType() const;
        
        static const objectType type = event;
        
        void serialize(ByteArray& out) const;
        void deserialize(ByteArray& in);
        
        bool isSystem() const;
        const Address& getDestination() const;
        uint64_t getSenderId() const;
        const Object& getData() const;
        
        void setSenderId(const Uint64& senderId);
        
    private:
        Boolean system;
        Address destination;
        Uint64 sender;
        Object* data;
        
    };
}

#endif // EVENT_H
