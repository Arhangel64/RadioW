#include "attributes.h"

M::Attributes::Attributes(const W::Address p_address, QObject* parent):
    M::Vocabulary(p_address, parent),
    attributes(new Map())
{
}

M::Attributes::~Attributes()
{
    delete attributes;
}

M::Model::ModelType M::Attributes::getType() const
{
    return type;
}

void M::Attributes::addAttribute(const W::String& key, M::Model* model)
{
    Map::const_iterator itr = attributes->find(key);
    if (itr != attributes->end()) {
        throw 1;
    }
    
    attributes->insert(std::make_pair(key, model));
    addModel(model);
    W::Vocabulary* vc = new W::Vocabulary();
    vc->insert(u"name", key);
    vc->insert(u"address", model->getAddress());
    vc->insert(u"type", W::Uint64(model->getType()));
    
    insert(key, vc);
}

void M::Attributes::removeAttribute(const W::String& key)
{
    Map::const_iterator itr = attributes->find(key);
    if (itr == attributes->end()) {
        throw 1;
    }
    M::Model* model = itr->second;
    attributes->erase(itr);
    erase(key);
    removeModel(model);
    
    delete model;
}

void M::Attributes::setAttribute(const W::String& key, const W::Object& value)
{
    Map::const_iterator itr = attributes->find(key);
    itr->second->set(value);
}

void M::Attributes::setAttribute(const W::String& key, W::Object* value)
{
    Map::const_iterator itr = attributes->find(key);
    itr->second->set(value);
}
