#include "collection.h"

C::Collection::Collection(const W::Address p_address, QObject* parent):
    C::Catalogue(p_address, parent),
    elements(),
    waitingElements(),
    hasData(false)
{
}

C::Collection::~Collection()
{
    
}

void C::Collection::addChildVocabulary(const W::Uint64& id)
{
    C::Vocabulary* ctrl = new C::Vocabulary(pairAddress + id);
    elements.insert(std::make_pair(id, ctrl));
    waitingElements.insert(ctrl);
    addController(ctrl);
    
    connect(ctrl, SIGNAL(data()), this, SLOT(onChildVCData()));
    
    if (hasData) {
        hasData = false;
    }
}

void C::Collection::addElement(const W::Uint64& id, const W::Uint64& before)
{
    C::Catalogue::addElement(id, before);
    addChildVocabulary(id);
}

void C::Collection::clearCatalogue()
{
    C::Catalogue::clearCatalogue();
    
    std::set<C::Vocabulary*>::const_iterator itr = waitingElements.begin();
    std::set<C::Vocabulary*>::const_iterator end = waitingElements.end();
    
    for (; itr != end; ++itr) {
        disconnect(*itr, SIGNAL(data()), this, SLOT(onChildVCData()));
    }
    
    elements.clear();
    waitingElements.clear();
    cleanChildren();
}

void C::Collection::removeElement(const W::Uint64& id)
{
    C::Catalogue::removeElement(id);
    
    Elements::const_iterator itr = elements.find(id);
    C::Vocabulary* ctrl = itr->second;
    
    removeController(ctrl);
    elements.erase(itr);
    
    if (!hasData) {
        std::set<C::Vocabulary*>::const_iterator witr = waitingElements.find(ctrl);
        if (witr != waitingElements.end()) {
            disconnect(ctrl, SIGNAL(data()), this, SLOT(onChildVCData()));
            waitingElements.erase(witr);
            
            if (waitingElements.size() == 0) {
                hasData = true;
                emit ready();
            }
        }
    }
    delete ctrl;
}

void C::Collection::h_get(const W::Event& ev)
{
    hasData = false;
    C::Catalogue::h_get(ev);
    
    if (waitingElements.size() == 0) {
        hasData = true;
        emit ready();
    }
}

void C::Collection::h_clear(const W::Event& ev)
{
    C::Catalogue::h_clear(ev);
    if (!hasData) {
        hasData = true;
        emit ready();
    }
}


void C::Collection::onChildVCData()
{
    C::Vocabulary* child = static_cast<C::Vocabulary*>(sender());
    
    std::set<C::Vocabulary*>::const_iterator itr = waitingElements.find(child);
    waitingElements.erase(itr);
    
    disconnect(child, SIGNAL(data()), this, SLOT(onChildVCData()));
    
    if (waitingElements.size() == 0) {
        hasData = true;
        emit ready();
    }
}

std::set<uint64_t> C::Collection::find(const W::String& field, const W::Object& value)
{
    if (!hasData) {
        emit serviceMessage(QString("An attempt to look for record where ") + field.toString().c_str() + " == " + value.toString().c_str() + " in " + address.toString().c_str() + " but controller has no data yet");
        throw 2;
    }
    
    std::set<uint64_t> response;
    Elements::const_iterator itr = elements.begin();
    Elements::const_iterator end = elements.end();
    
    for (; itr != end; ++itr) {
        if (itr->second->at(field) == value) {
            response.insert(itr->first);
        }
    }
    
    return response;
}

const C::Vocabulary & C::Collection::get(uint64_t id) const
{
    return *(elements.find(id)->second);
}
