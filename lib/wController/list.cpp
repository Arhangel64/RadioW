#include "list.h"

uint64_t C::List::counter = 0;

C::List::List(const W::Address p_address, QObject* parent):
    C::Controller(p_address, W::Address({W::String(u"list") += counter++}), parent),
    data()
{
    W::Handler* get = W::Handler::create(address + W::Address({u"get"}), this, &C::List::_h_get);
    W::Handler* push = W::Handler::create(address + W::Address({u"push"}), this, &C::List::_h_push);
    W::Handler* clear = W::Handler::create(address + W::Address({u"clear"}), this, &C::List::_h_clear);
    addHandler(get);
    addHandler(push);
    addHandler(clear);
}

C::List::~List()
{
}


void C::List::h_get(const W::Event& ev)
{
    emit clear();
    
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    data = static_cast<const W::Vector&>(vc.at(u"data"));
    
    int size = data.size();
    for (int i = 0; i < size; ++i) {
        emit newElement(data.at(i));
    }
}

void C::List::h_push(const W::Event& ev)
{
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    const W::Object& el = vc.at(u"data");
    
    data.push(el);
    emit newElement(el);
}

void C::List::h_clear(const W::Event& ev)
{
    emit clear();
    data.clear();
}
