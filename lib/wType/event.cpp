#include "event.h"

W::Event::Event():
    Object(),
    system(false),
    destination(),
    sender(0),
    data(0)
{

}

W::Event::Event(const W::Address& p_addr, const W::Object& p_data, uint64_t p_sender, bool p_system):
    Object(),
    system(p_system),
    destination(p_addr),
    sender(p_sender),
    data(p_data.copy())
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

W::Object::size_type W::Event::size() const
{
    size_type my_size = 3;
    my_size += destination.size();
    my_size += data->size();
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
    destination.serialize(out);
    sender.serialize(out);
    
    out << *data;
}

void W::Event::deserialize(W::ByteArray& in)
{
    system.deserialize(in);
    destination.deserialize(in);
    sender.deserialize(in);
    
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