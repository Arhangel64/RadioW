#include "event.h"

W::Event::Event():
    Object(),
    system(false),
    destination(),
    sender(0),
    data(0)
{

}

W::Event::Event(const W::Address& p_addr, const W::Object& p_data, bool p_system):
    Object(),
    system(p_system),
    destination(p_addr),
    sender(0),
    data(p_data.copy())
{

}

W::Event::Event(const W::Address& p_addr, W::Object* p_data, bool p_system):
    Object(),
    system(p_system),
    destination(p_addr),
    sender(0),
    data(p_data)
{

}


W::Event::Event(const W::Event& original):
    Object(),
    system(original.system),
    destination(original.destination),
    sender(original.sender),
    data(original.data->copy())
{

}

W::Event::~Event()
{
    delete data;
}

W::Event& W::Event::operator=(const W::Event& original)
{
    if (&original != this)
    {
        delete data;
        
        system = original.system;
        destination = original.destination;
        sender = original.sender;
        data = original.data->copy();
    }
    return *this;
}

W::Object* W::Event::copy() const
{
    return new Event(*this);
}

W::Object::objectType W::Event::getType() const
{
    return Event::type;
}

W::Object::size_type W::Event::length() const
{
    size_type my_size = 2;
    my_size += destination.length();
    my_size += data->length();
    return my_size;
}

W::Object::StdStr W::Event::toString() const
{
    StdStr result;
    
    result += "{";
    
    result += "system: ";
    result += system.toString();
    
    result += ", desitnation: ";
    result += destination.toString();
    
    result += ", sender: ";
    result += sender.toString();
    
    result += ", data: ";
    result += data->toString();
    
    result += "}";
    
    return result;
}

void W::Event::serialize(W::ByteArray& out) const
{
    system.serialize(out);
    if (!system)
    {
        destination.serialize(out);
        sender.serialize(out);
    }
    
    out.push8(data->getType());
    data->serialize(out);
}

void W::Event::deserialize(W::ByteArray& in)
{
    system.deserialize(in);
    if (!system)
    {
        destination.deserialize(in);
        sender.deserialize(in);
    }
    
    delete data;
    
    data = Object::fromByteArray(in);
}

bool W::Event::isSystem() const
{
    return system;
}

const W::Address& W::Event::getDestination() const
{
    return destination;
}

uint64_t W::Event::getSenderId() const
{
    return sender;
}

const W::Object& W::Event::getData() const
{
    return *data;
}

void W::Event::setSenderId(const W::Uint64& senderId)
{
    sender = senderId;
}

void W::Event::setSenderId(uint64_t senderId)
{
    sender = Uint64(senderId);
}

bool W::Event::operator==(const W::Object& other) const
{
    if (sameType(other)) {
        const W::Event& oev = static_cast<const W::Event&>(other);
        return destination == oev.destination && system == oev.system && sender == oev.sender && *data == *(oev.data);
    } else {
        return false;
    }
}

W::Object::size_type W::Event::size() const
{
    size_type sz = system.size() + data->size() + 1;
    if (!system)
    {
        sz += destination.size() + sender.size();
    }
    return sz;
}
