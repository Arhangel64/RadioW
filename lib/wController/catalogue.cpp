#include "catalogue.h"

uint64_t C::Catalogue::counter = 0;

C::Catalogue::Catalogue(const W::Address p_address, QObject* parent):
    C::Controller(p_address, W::Address({W::String(u"catalogue") += counter++}), parent),
    order(),
    hasSorting(false),
    hasFilter(false),
    hasData(true),
    sorting(0),
    filter(0)
{
    W::Handler* get = W::Handler::create(address + W::Address({u"get"}), this, &C::Catalogue::_h_get);
    W::Handler* addElement = W::Handler::create(address + W::Address({u"addElement"}), this, &C::Catalogue::_h_addElement);
    W::Handler* removeElement = W::Handler::create(address + W::Address({u"removeElement"}), this, &C::Catalogue::_h_removeElement);
    W::Handler* moveElement = W::Handler::create(address + W::Address({u"moveElement"}), this, &C::Catalogue::_h_moveElement);
    W::Handler* clear = W::Handler::create(address + W::Address({u"clear"}), this, &C::Catalogue::_h_clear);
    
    addHandler(get);
    addHandler(addElement);
    addHandler(removeElement);
    addHandler(moveElement);
    addHandler(clear);
}

C::Catalogue::~Catalogue()
{
    if (hasFilter) {
        delete filter;
    }
    
    if (hasSorting) {
        delete sorting;
    }
}

void C::Catalogue::setSorting(const W::String& field, bool ascending)
{
    if (!hasSorting) {
        sorting = new W::Vocabulary();
        hasSorting = true;
    }
    sorting->insert(u"field", field);
    sorting->insert(u"ascending", W::Boolean(ascending));
    
    if (hasData) {
        clearCatalogue();
    }
    
    if (subscribed) {
        getData();
    }
}

void C::Catalogue::clearSorting()
{
    if (hasSorting) {
        delete sorting;
        hasSorting = false;
        
        if (hasData) {
            clearCatalogue();
        }
        
        if (subscribed) {
            getData();
        }
    }
}

void C::Catalogue::addElement(const W::Uint64& id, const W::Uint64& before)
{
    if (before == 0) {
        order.push_back(id);
    } else {
        W::Order<uint64_t>::const_iterator pos = order.find(before);
        order.insert(pos, id);
    }
    
    emit addedElement(id, before);
}

void C::Catalogue::h_get(const W::Event& ev)
{
    if (hasData) {
        clearCatalogue();
    }
    
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    const W::Vector& ord = static_cast<const W::Vector&>(vc.at(u"data"));
    
    W::Vector::size_type size = ord.size();
    for (uint64_t i = 0; i < size; ++i) {
        const W::Uint64& id = static_cast<const W::Uint64&>(ord.at(i));
        addElement(id);
    }
    hasData = true;
    
    emit data();
}

void C::Catalogue::h_addElement(const W::Event& ev)
{
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    const W::Uint64& id = static_cast<const W::Uint64&>(vc.at(u"id"));
    if (vc.has(u"before")) {
        const W::Uint64& before = static_cast<const W::Uint64&>(vc.at(u"before"));
        
        addElement(id, before);
    } else {
        addElement(id);
    }
}

void C::Catalogue::clearCatalogue()
{
    order.clear();
    hasData = false;
    emit clear();
}


void C::Catalogue::h_clear(const W::Event& ev)
{
    clearCatalogue();
}

void C::Catalogue::h_removeElement(const W::Event& ev)
{
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    const W::Uint64& id = static_cast<const W::Uint64&>(vc.at(u"id"));
    
    removeElement(id);
}

void C::Catalogue::removeElement(const W::Uint64& id)
{
    W::Order<uint64_t>::const_iterator pos = order.find(id);
    if (pos == order.end()) {
        emit serviceMessage(QString("Recieved event to remove element with id ") + id.toString().c_str() + " but element under such id isn't present in catalogue, skipping");
        return;
    }
    order.erase(id);
    
    uint64_t pid;
    emit removedElement(pid);
}

W::Vocabulary * C::Catalogue::createSubscriptionVC() const
{
    W::Vocabulary* vc = C::Controller::createSubscriptionVC();
    
    if (hasSorting) {
        vc->insert(u"sorting", sorting->copy());
    }
    
    if (hasFilter) {
        vc->insert(u"filter", filter->copy());
    }
    
    return vc;
}

void C::Catalogue::h_moveElement(const W::Event& ev)
{
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    const W::Uint64& id = static_cast<const W::Uint64&>(vc.at(u"id"));
    
    W::Order<uint64_t>::const_iterator pos = order.find(id);
    if (pos == order.end()) {
        emit serviceMessage(QString("Recieved event to move element with id ") + id.toString().c_str() + " but element under such id isn't present in catalogue, skipping");
        return;
    }
    
    order.erase(id);
    if (vc.has(u"before")) {
        const W::Uint64& before = static_cast<const W::Uint64&>(vc.at(u"before"));
        
        W::Order<uint64_t>::const_iterator beforePosition = order.find(before);
        if (beforePosition == order.end()) {
            emit serviceMessage(QString("Recieved event to move element with id ") +
                                        id.toString().c_str() + " before element with id " + before.toString().c_str() +
                                        " but element under id " + before.toString().c_str() +
                                        " isn't present in catalogue, inserting to the end");
            
            order.push_back(id);
            emit movedElement(id);
            
            return;
        }
        order.insert(beforePosition, id);
        emit movedElement(id, before);
        
    } else {
        order.push_back(id);
        emit movedElement(id);
    }
}

void C::Catalogue::getData()
{
    W::Vocabulary* vc =  new W::Vocabulary();
    vc->insert(u"params", createSubscriptionVC());
    send(vc, W::Address{u"get"});
}


void C::Catalogue::addRemoteElement(const W::Vocabulary& element) const
{
    send(static_cast<W::Vocabulary*>(element.copy()), W::Address{u"add"});
}

void C::Catalogue::updateRemoteElement(const W::Uint64& id, const W::Vocabulary& newValue) const
{
    W::Vocabulary* vc =  new W::Vocabulary();
    vc->insert(u"id", id);
    vc->insert(u"value", newValue);
    send(vc, W::Address{u"update"});
}
