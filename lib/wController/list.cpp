#include "list.h"

uint64_t C::List::counter = 0;

C::List::List(const W::Address p_address, QObject* parent):
    C::Controller(p_address, W::Address({W::String(u"list") += counter++}), parent),
    data(new W::Vector())
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
    delete data;
}


void C::List::h_get(const W::Event& ev)
{
    emit clear();
    data->clear();
    
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    const W::Vector& edata = static_cast<const W::Vector&>(vc.at(u"data"));
    
    int size = edata.size();
    for (int i = 0; i < size; ++i) {
        data->push(edata.at(i));
        emit newElement(edata.at(i));
    }
    
    emit modification(*data);
}

void C::List::h_push(const W::Event& ev)
{
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    const W::Object& el = vc.at(u"data");
    
    data->push(el);
    emit newElement(el);
    
    emit modification(*data);
}

void C::List::h_clear(const W::Event& ev)
{
    emit clear();
    data->clear();
    
    emit modification(*data);
}
