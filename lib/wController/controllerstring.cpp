#include "controllerstring.h"

uint64_t C::String::counter = 0;

C::String::String(const W::Address p_address, QObject* parent):
    C::Controller(p_address, W::Address({W::String(u"string") + counter++}), parent),
    data(u"")
{
    W::Handler* get = W::Handler::create(address + W::Address({u"get"}), this, &C::String::_h_get);
    addHandler(get);
}

C::String::~String()
{
}

void C::String::h_get(const W::Event& ev)
{
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    
    data = static_cast<const W::String&>(vc.at(u"data"));
    
    emit change(QString(data.toString().c_str()));
}
