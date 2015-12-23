#ifndef EVENT_H
#define EVENT_H

#include "object.h"
#include "address.h"
#include "boolean.h"

namespace W
{
    class Event:
        public Object
    {
    public:
        Event();
        Event(const Address& p_addr, const Object& p_data, bool p_system = false);
        Event(const Event& original);
        ~Event();
        
        Event& operator=(const Event& original);
        
        StdStr toString() const;
        Object* copy() const;
        size_type size() const;
        objectType getType() const;
        
        static const objectType type = event;
        
    private:
        Boolean system;
        Address destination;
        Object* data;
        
    };
}

#endif // EVENT_H
