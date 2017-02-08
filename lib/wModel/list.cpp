#include "list.h"

M::List::List(const W::Address p_address, QObject* parent):
    M::Model(p_address, parent),
    data(new W::Vector())
{
    W::Handler* get = W::Handler::create(address + W::Address({u"get"}), this, &M::List::_h_get);
    addHandler(get);
}

M::List::~List()
{
    delete data;
}

void M::List::h_subscribe(const W::Event& ev)
{
    M::Model::h_subscribe(ev);
    
    h_get(ev);
}

void M::List::h_get(const W::Event& ev)
{
    W::Vocabulary* vc = new W::Vocabulary;
    vc->insert(u"data", data->copy());
    
    response(vc, W::Address({u"get"}), ev);
}

void M::List::push(const W::Object& obj)
{
    data->push(obj);
    
    if (registered) {
        W::Vocabulary* vc = new W::Vocabulary();
        vc->insert(u"data", obj);
        
        broadcast(vc, W::Address{u"push"});
    }
}

void M::List::push(W::Object* obj)
{
    data->push(obj);
    
    if (registered) {
        W::Vocabulary* vc = new W::Vocabulary();
        vc->insert(u"data", obj->copy());
        
        broadcast(vc, W::Address{u"push"});
    }
}

void M::List::clear()
{
    data->clear();
    
    if (registered) {
        broadcast(new W::Vocabulary(), W::Address{u"clear"});
    }
}

M::Model::ModelType M::List::getType() const
{
    return type;
}

void M::List::set(const W::Object& value)
{
    delete data;
    data = static_cast<W::Vector*>(value.copy());
    
    W::Vocabulary* vc = new W::Vocabulary;
    vc->insert(u"data", data->copy());
    
    broadcast(vc, W::Address({u"get"}));
}

void M::List::set(W::Object* value)
{
    delete data;
    data = static_cast<W::Vector*>(value);
    
    W::Vocabulary* vc = new W::Vocabulary;
    vc->insert(u"data", data->copy());
    
    broadcast(vc, W::Address({u"get"}));
}

uint64_t M::List::size() const
{
    return data->size();
}

const W::Object & M::List::at(uint64_t index) const
{
    return data->at(index);
}

