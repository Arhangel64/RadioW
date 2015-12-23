#include "event.h"

W::Event::Event():
    Object(),
    system(false),
    destination(),
    data(0)
{

}

W::Event::Event(const W::Address& p_addr, const W::Object& p_data, bool p_system):
    Object(),
    system(p_system),
    destination(p_addr),
    data(p_data.copy())
{

}

W::Event::Event(const W::Event& original):
    Object(),
    system(original.system),
    destination(original.destination),
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
    size_type my_size = 0;
    my_size += address.size();
    my_size += data->size();
    return my_size;
}

W::Object::StdStr W::Event::toString() const
{
    
}
