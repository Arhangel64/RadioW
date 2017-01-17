#include "attributes.h"

uint64_t C::Attributes::counter = 0;

C::Attributes::Attributes(const W::Address& p_address, QObject* parent):
    C::Vocabulary(p_address, W::Address({W::String(u"attributes") += counter++}), parent),
    attributes(new Map()),
    reversed(new RMap())
{
}

C::Attributes::~Attributes()
{
    delete attributes;
    delete reversed;
}

void C::Attributes::_newElement(const W::String& key, const W::Object& element)
{
    const W::Vocabulary& evc = static_cast<const W::Vocabulary&>(element);
    const W::Uint64& type = static_cast<const W::Uint64&>(evc.at(u"type"));
    const W::Address& addr = static_cast<const W::Address&>(evc.at(u"address"));
    
    C::Controller* child = C::Controller::createByType(type, addr);
    attributes->insert(std::make_pair(key, child));
    reversed->insert(std::make_pair(child, key));
    addController(child);
    connect(child, SIGNAL(modification(const W::Object&)), SLOT(onAttrModification(const W::Object&)));
    
    C::Vocabulary::_newElement(key, element);
}

void C::Attributes::_removeElement(const W::String& key)
{
    C::Vocabulary::_removeElement(key);
    
    Map::iterator itr = attributes->find(key);
    C::Controller* ctrl = itr->second;
    ctrl->setProperty("name", QString::fromStdString(key.toString()));
    RMap::iterator ritr = reversed->find(ctrl);
    
    removeController(ctrl);
    attributes->erase(itr);
    reversed->erase(ritr);
    delete ctrl;
}

void C::Attributes::_clear()
{
    C::Vocabulary::_clear();
    
    Map::iterator itr = attributes->begin();
    Map::iterator end = attributes->end();
    
    for (; itr != end; ++itr) {
        removeController(itr->second);
        delete itr->second;
    }
    
    attributes->clear();
    reversed->clear();
}


void C::Attributes::onAttrModification(const W::Object& data)
{
    C::Controller* ctrl = static_cast<C::Controller*>(sender());
    
    RMap::iterator ritr = reversed->find(ctrl);
    
    emit attributeChange(ritr->second, data);
}

void C::Attributes::unsubscribe()
{
    C::Controller::unsubscribe();
    
    _clear();
}

void C::Attributes::onSocketDisconnected()
{
    C::Controller::onSocketDisconnected();
    
    dropSubscribed();
    _clear();
}
