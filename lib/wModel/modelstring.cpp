#include "modelstring.h"

M::String::String(const W::String& str, const W::Address& addr, QObject* parent):
    M::Model(addr, parent),
    data(new W::String(str))
{
    addHandler(W::Handler::create(address + W::Address({u"get"}), this, &M::String::_h_get));
}

M::String::String(W::String* str, const W::Address& addr, QObject* parent):
    M::Model(addr, parent),
    data(str)
{
}

M::String::~String()
{
    delete data;
}

void M::String::h_subscribe(const W::Event& ev)
{
    M::Model::h_subscribe(ev);
    
    h_get(ev);
}

void M::String::h_get(const W::Event& ev)
{
    W::Vocabulary* vc = new W::Vocabulary;
    vc->insert(u"data", *data);
    
    response(vc, W::Address({u"get"}), ev);
}

void M::String::set(const W::String& str)
{
    delete data;
    data = static_cast<W::String*>(str.copy());
    
    if (registered) {
        W::Vocabulary* vc = new W::Vocabulary();
        vc->insert(u"data", str);
    
        broadcast(vc, W::Address{u"get"});
    }
}

void M::String::set(W::String* str)
{
    delete data;
    data = str;
    
    if (registered) {
        W::Vocabulary* vc = new W::Vocabulary();
        vc->insert(u"data", *str);
        
        broadcast(vc, W::Address{u"get"});
    }
}
