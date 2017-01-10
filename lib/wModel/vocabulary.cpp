#include "vocabulary.h"

M::Vocabulary::Vocabulary(const W::Address p_address, QObject* parent):
    M::Model(p_address, parent),
    data(new W::Vocabulary())
{
    W::Handler* get = W::Handler::create(address + W::Address({u"get"}), this, &M::Vocabulary::_h_get);
    addHandler(get);
}

M::Vocabulary::~Vocabulary()
{
    delete data;
}

void M::Vocabulary::h_subscribe(const W::Event& ev)
{
    M::Model::h_subscribe(ev);
    
    h_get(ev);
}

void M::Vocabulary::h_get(const W::Event& ev)
{
    W::Vocabulary* vc = new W::Vocabulary;
    vc->insert(u"data", *data);
    
    response(vc, W::Address({u"get"}), ev);
}

void M::Vocabulary::insert(const W::String& key, const W::Object& value)
{
    if (registered) {
        W::Vocabulary* vc = new W::Vocabulary();
        W::Vocabulary* insert = new W::Vocabulary();
        W::Vector* erase = new W::Vector();
        
        if (data->has(key)) {
            erase->push(key);
        }
        data->insert(key, value);
        insert->insert(key, value);
        
        vc->insert(u"insert", insert);
        vc->insert(u"erase", erase);
        
        broadcast(vc, W::Address{u"change"});
    } else {
        data->insert(key, value);
    }
}

void M::Vocabulary::insert(const W::String& key, W::Object* value)
{
    if (registered) {
        W::Vocabulary* vc = new W::Vocabulary();
        W::Vocabulary* insert = new W::Vocabulary();
        W::Vector* erase = new W::Vector();
        
        if (data->has(key)) {
            erase->push(key);
        }
        data->insert(key, value);
        insert->insert(key, value);
        
        vc->insert(u"insert", insert);
        vc->insert(u"erase", erase);
        
        broadcast(vc, W::Address{u"change"});
    } else {
        data->insert(key, value);
    }
}

void M::Vocabulary::erase(const W::String& key)
{
    data->erase(key);
    if (registered) {
        W::Vocabulary* vc = new W::Vocabulary();
        W::Vocabulary* insert = new W::Vocabulary();
        W::Vector* erase = new W::Vector();
            
        erase->push(key);
            
        vc->insert(u"insert", insert);
        vc->insert(u"erase", erase);
        
        broadcast(vc, W::Address{u"change"});
    }
}

void M::Vocabulary::clear()
{
    data->clear();
    
    if (registered) {
        broadcast(new W::Vocabulary(), W::Address{u"clear"});
    }
}
