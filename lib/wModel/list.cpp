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
    vc->insert(u"data", *data);
    
    response(vc, W::Address({u"get"}), ev);
}

void M::List::push(const W::Object& obj)
{
    data->push(obj);
    
    W::Vocabulary* vc = new W::Vocabulary();
    vc->insert(u"data", obj);
    
    broadcast(vc, W::Address{u"push"});
}

void M::List::push(W::Object* obj)
{
    data->push(obj);
    
    W::Vocabulary* vc = new W::Vocabulary();
    vc->insert(u"data", *obj);
    
    broadcast(vc, W::Address{u"push"});
}

void M::List::clear()
{
    data->clear();
    
    broadcast(new W::Vocabulary(), W::Address{u"clear"});
}
