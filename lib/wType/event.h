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
        
        StdStr toString() const override;
        Object* copy() const override;
        size_type size() const override;
        objectType getType() const override;
        
        bool operator==(const W::Object & other) const override;
        
        static const objectType type = event;
        
        void serialize(ByteArray& out) const override;
        void deserialize(ByteArray& in) override;
        
        bool isSystem() const;
        const Address& getDestination() const;
        uint64_t getSenderId() const;
        const Object& getData() const;
        
        void setSenderId(const Uint64& senderId);
        void setSenderId(uint64_t senderId);
        
    private:
        Boolean system;
        Address destination;
        Uint64 sender;
        Object* data;
        
    };
}

#endif // EVENT_H
