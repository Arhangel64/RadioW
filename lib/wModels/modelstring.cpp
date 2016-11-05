#include "modelstring.h"

M::String::String(const W::String& str, const W::Address& addr, QObject* parent):
    M::Model(addr, parent),
    data(new W::String(str))
{
    addHandler(W::Handler::create(address + W::Address({u"get"}), this, &M::String::h_get));
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
