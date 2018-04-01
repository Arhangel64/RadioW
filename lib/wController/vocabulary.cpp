#include "vocabulary.h"

uint64_t C::Vocabulary::counter = 0;

C::Vocabulary::Vocabulary(const W::Address p_address, QObject* parent):
    C::Controller(p_address, W::Address({W::String(u"vocabulary") += counter++}), parent),
    p_data(new W::Vocabulary())
{
    W::Handler* get = W::Handler::create(address + W::Address({u"get"}), this, &C::Vocabulary::_h_get);
    W::Handler* change = W::Handler::create(address + W::Address({u"change"}), this, &C::Vocabulary::_h_change);
    W::Handler* clear = W::Handler::create(address + W::Address({u"clear"}), this, &C::Vocabulary::_h_clear);
    addHandler(get);
    addHandler(change);
    addHandler(clear);
}

C::Vocabulary::~Vocabulary()
{
    delete p_data;
}

C::Vocabulary::Vocabulary(const W::Address p_address, const W::Address& my_address, QObject* parent):
    C::Controller(p_address, my_address, parent),
    p_data(new W::Vocabulary())
{
    W::Handler* get = W::Handler::create(address + W::Address({u"get"}), this, &C::Vocabulary::_h_get);
    W::Handler* change = W::Handler::create(address + W::Address({u"change"}), this, &C::Vocabulary::_h_change);
    W::Handler* clear = W::Handler::create(address + W::Address({u"clear"}), this, &C::Vocabulary::_h_clear);
    addHandler(get);
    addHandler(change);
    addHandler(clear);
}


void C::Vocabulary::h_get(const W::Event& ev)
{
    _clear();
    
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    const W::Vocabulary& e_data = static_cast<const W::Vocabulary&>(vc.at(u"data"));
    
    W::Vector keys = e_data.keys();
    int size = keys.length();
    for (int i = 0; i < size; ++i) {
        const W::String& key = static_cast<const W::String&>(keys.at(i));
        _newElement(key, e_data.at(key));
    }
    
    emit modification(*p_data);
    emit data();
}

void C::Vocabulary::h_change(const W::Event& ev)
{
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    
    const W::Vector& erase = static_cast<const W::Vector&>(vc.at(u"erase"));
    const W::Vocabulary& insert = static_cast<const W::Vocabulary&>(vc.at(u"insert"));
    
    int eSize = erase.length();
    for (int i = 0; i < eSize; ++i) {
        const W::String& key = static_cast<const W::String&>(erase.at(i));
        _removeElement(key);
    }
    
    W::Vector keys = insert.keys();
    int iSize = keys.length();
    for (int i = 0; i < iSize; ++i) {
        const W::String& key = static_cast<const W::String&>(keys.at(i));
        _newElement(key, insert.at(key));
    }
    
    emit modification(*p_data);
}

void C::Vocabulary::h_clear(const W::Event& ev)
{
    _clear();
    emit modification(*p_data);
}

void C::Vocabulary::_newElement(const W::String& key, const W::Object& element)
{
    p_data->insert(key, element);
    emit newElement(key, element);
}

void C::Vocabulary::_removeElement(const W::String& key)
{
    emit removeElement(key);
    p_data->erase(key);
}


void C::Vocabulary::_clear()
{
    emit clear();
    p_data->clear();
}

const W::Object & C::Vocabulary::at(const W::String& key) const
{
    return p_data->at(key);
}

const W::Object & C::Vocabulary::at(const W::String::u16string& key) const
{
    return p_data->at(key);
}

bool C::Vocabulary::has(const W::String& key) const
{
    return p_data->has(key);
}

bool C::Vocabulary::has(const W::String::u16string& key) const
{
    return p_data->has(key);
}
