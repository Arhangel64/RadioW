#include "icatalogue.h"

const std::set<uint64_t> M::ICatalogue::empty = std::set<uint64_t>();

M::ICatalogue::ICatalogue(const W::Address p_address, QObject* parent):
    M::Model(p_address, parent),
    subscribeMember(W::Handler::create(W::Address({}), this, &M::ICatalogue::_h_subscribeMember)),
    indexes(),
    lastIndex(0),
    activeChildren()
{
    W::Handler* get = W::Handler::create(address + W::Address({u"get"}), this, &M::ICatalogue::_h_get);
    W::Handler* add = W::Handler::create(address + W::Address({u"add"}), this, &M::ICatalogue::_h_add);
    W::Handler* update = W::Handler::create(address + W::Address({u"update"}), this, &M::ICatalogue::_h_update);
    
    addHandler(get);
    addHandler(add);
    addHandler(update);
}

M::ICatalogue::~ICatalogue()
{
    delete subscribeMember;
    
    IndexMap::const_iterator itr = indexes.begin();
    IndexMap::const_iterator end = indexes.end();
    
    for (; itr != end; ++itr) {
        delete itr->second;
    }
}

void M::ICatalogue::clear()
{
    lastIndex = 0;
    IndexMap::const_iterator itr = indexes.begin();
    IndexMap::const_iterator end = indexes.end();
    
    for (; itr != end; ++itr) {
        itr->second->clear();
    }
    
    if (registered) {
        broadcast(new W::Vocabulary(), W::Address{u"clear"});
    }
    
    emit countChange(0);
}


void M::ICatalogue::addIndex(const W::String& fieldName, W::Object::objectType fieldType)
{
    IndexMap::const_iterator itr = indexes.find(fieldName);
    if (itr != indexes.end()) {
        throw 2;
    }
    
    switch (fieldType) {
        case W::Object::uint64:
            indexes.insert(std::make_pair(fieldName, new Index<W::Uint64>()));
            break;
        case W::Object::string:
            indexes.insert(std::make_pair(fieldName, new Index<W::String>()));
            break;
            
        default:
            throw 3;
    }
}

const std::set<uint64_t> & M::ICatalogue::find(const W::String& indexName, const W::Object& value) const
{
    IndexMap::const_iterator itr = indexes.find(indexName);
    if (itr == indexes.end()) {
        throw 4;
    }
    
    return itr->second->find(value);
}


std::set<uint64_t> M::ICatalogue::find(const W::Vocabulary& value) const
{
    W::Vector keys = value.keys();
    int size = keys.length();
    
    std::set<uint64_t> result;
    bool first = true;
    
    for (int i = 0; i < size; ++i) {
        const W::String& key = static_cast<const W::String&>(keys.at(i));
        IndexMap::const_iterator itr = indexes.find(key);
        if (itr == indexes.end()) {
            throw 4;
        }
        if (first) {
            result = itr->second->find(value.at(key));
            first = false;
        } else {
            std::set<uint64_t> copy = result;
            result.clear();
            const std::set<uint64_t>& current = itr->second->find(value.at(key));
            std::set_intersection(copy.begin(), copy.end(), current.begin(), current.end(), std::inserter(result, result.end()));
        }
        
        if (result.empty()) {
            break;
        }
    }
    
    return result;
}

M::Model::ModelType M::ICatalogue::getType() const
{
    return type;
}

uint64_t M::ICatalogue::addElement(const W::Vocabulary& record)
{
    IndexMap::const_iterator itr = indexes.begin();
    IndexMap::const_iterator end = indexes.end();
    
    ++lastIndex;
    
    for (; itr != end; ++itr) {
        itr->second->add(record.at(itr->first), lastIndex);
    }
    
    Map::const_iterator sItr = subscribers->begin();
    Map::const_iterator sEnd = subscribers->end();
    
    for (; sItr != sEnd; ++sItr) {
        SMap::const_iterator oItr = sItr->second.begin();
        SMap::const_iterator oEnd = sItr->second.end();
        
        for (; oItr != oEnd; ++oItr) {
            const W::Vocabulary& params = oItr->second;
            if (params.has(u"filter")) {
                processAddElement(lastIndex, record, oItr, sItr->first);
            } else {
                uint64_t bid = getInsertingNeighbour(oItr->second, record, lastIndex, getAll());
                
                W::Address dest = oItr->first + W::Address({u"addElement"});
                W::Vocabulary* vc = new W::Vocabulary();
                vc->insert(u"id", new W::Uint64(lastIndex));
                if (bid != 0) {
                    vc->insert(u"before", new W::Uint64(bid));
                }
                send(vc, dest, sItr->first);
            }
        }
    }
    
    emit countChange(size() + 1);
    return lastIndex;
}

void M::ICatalogue::removeElement(uint64_t id)
{
    IndexMap::const_iterator itr = indexes.begin();
    IndexMap::const_iterator end = indexes.end();
    
    W::Vocabulary* value = getElement(id);
    
    Map::const_iterator sItr = subscribers->begin();
    Map::const_iterator sEnd = subscribers->end();
    
    for (; sItr != sEnd; ++sItr) {
        SMap::const_iterator oItr = sItr->second.begin();
        SMap::const_iterator oEnd = sItr->second.end();
        
        for (; oItr != oEnd; ++oItr) {
            const W::Vocabulary& params = oItr->second;
            if (params.has(u"filter")) {
                const W::Vocabulary& filter = static_cast<const W::Vocabulary&>(params.at(u"filter"));
                std::set<uint64_t> set = find(filter);
                std::set<uint64_t>::const_iterator idItr = set.find(id);
                if (idItr != set.end()) {         
                    W::Address dest = oItr->first + W::Address({u"removeElement"});
                    W::Vocabulary* vc = new W::Vocabulary();
                    vc->insert(u"id", new W::Uint64(id));
                    send(vc, dest, sItr->first);
                }
            } else {
                W::Address dest = oItr->first + W::Address({u"removeElement"});
                W::Vocabulary* vc = new W::Vocabulary();
                vc->insert(u"id", new W::Uint64(id));
                send(vc, dest, sItr->first);
            }
        }
    }
    
    for (; itr != end; ++itr) {
        itr->second->remove(value->at(itr->first), id);
    }
    
    std::map<uint64_t, M::Vocabulary*>::const_iterator aItr = activeChildren.find(id);
    if (aItr != activeChildren.end()) {
        removeModel(aItr->second);
        aItr->second->deleteLater();
        activeChildren.erase(aItr);
    }
    
    emit countChange(size() - 1);
    delete value;
}

void M::ICatalogue::h_get(const W::Event& ev)
{
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    const W::Vocabulary& params = static_cast<const W::Vocabulary&>(vc.at(u"params"));
    
    std::set<uint64_t> set;
    if (params.has(u"filter")) {
        const W::Vocabulary& filter = static_cast<const W::Vocabulary&>(params.at(u"filter"));
        set = find(filter);
    } else {
        set = getAll();
    }
    
    W::Vocabulary* rvc = new W::Vocabulary;
    if (params.has(u"sorting")) {
        const W::Vocabulary& sorting = static_cast<const W::Vocabulary&>(params.at(u"sorting"));
        const W::String& field = static_cast<const W::String&>(sorting.at(u"field"));
        bool ascending = static_cast<const W::Boolean&>(sorting.at(u"ascending"));
        rvc->insert(u"data", indexes.at(field)->sort(set, ascending));
    } else {
        W::Vector* order = new W::Vector();
        std::set<uint64_t>::const_iterator itr = set.begin();
        std::set<uint64_t>::const_iterator end = set.end();
        
        for (; itr != end; ++itr) {
            order->push(W::Uint64(*itr));
        }
        rvc->insert(u"data", order);
    }
    
    response(rvc, W::Address({u"get"}), ev);
}

void M::ICatalogue::h_subscribe(const W::Event& ev)
{
    M::Model::h_subscribe(ev);
    
    h_get(ev);
}

void M::ICatalogue::set(const W::Object& value)
{
    throw 14;   //what do you expect here? not implemented, and not sure it ever would be
}

void M::ICatalogue::set(W::Object* value)
{
    set(*value);
}

void M::ICatalogue::h_add(const W::Event& ev)
{
    addElement(static_cast<const W::Vocabulary&>(ev.getData()));
}

void M::ICatalogue::h_update(const W::Event& ev)
{
    const W::Vocabulary& data = static_cast<const W::Vocabulary&>(ev.getData());
    
    const W::Uint64& id = static_cast<const W::Uint64&>(data.at(u"id"));
    const W::Vocabulary& newValue = static_cast<const W::Vocabulary&>(data.at(u"value"));
    W::Vector affectedKeys = newValue.keys();
    
    W::Vocabulary* oldValue = getElement(id);
    W::Vocabulary* modifiedValue = W::Vocabulary::extend(*oldValue, newValue);
    
    modifyElement(id, *modifiedValue);
    
    std::map<uint64_t, M::Vocabulary*>::const_iterator itr = activeChildren.find(id);
    if (itr != activeChildren.end()) {
        itr->second->set(modifiedValue);
    }
    
    Map::const_iterator sItr = subscribers->begin();
    Map::const_iterator sEnd = subscribers->end();
    
    for (; sItr != sEnd; ++sItr) {
        SMap::const_iterator oItr = sItr->second.begin();
        SMap::const_iterator oEnd = sItr->second.end();
        
        for (; oItr != oEnd; ++oItr) {
            const W::Vocabulary& params = oItr->second;
            if (params.has(u"filter")) {
                const W::Vocabulary& filter = static_cast<const W::Vocabulary&>(params.at(u"filter"));
                bool matched = match(*oldValue, filter);
                bool matching = match(*modifiedValue, filter);
                
                if (matched && !matching) {
                    W::Address dest = oItr->first + W::Address({u"removeElement"});
                    W::Vocabulary* vc = new W::Vocabulary();
                    vc->insert(u"id", new W::Uint64(id));
                    send(vc, dest, sItr->first);
                } else if (!matched && matching) {
                    processAddElement(id, *modifiedValue, oItr, sItr->first);
                } else if (matched && matching) {
                    std::set<uint64_t> set = find(filter);
                    uint64_t cbid = getInsertingNeighbour(params, *oldValue, id, set);
                    uint64_t bid = getInsertingNeighbour(params, *modifiedValue, id, set);
                    
                    if (cbid != bid) {
                        W::Address dest = oItr->first + W::Address({u"moveElement"});
                        W::Vocabulary* vc = new W::Vocabulary();
                        vc->insert(u"id", new W::Uint64(id));
                        if (id != 0) {
                            vc->insert(u"before", new W::Uint64(bid));
                        }
                        send(vc, dest, sItr->first);
                    }
                } 
            } else {
                if (params.has(u"sorting")) {
                    std::set<uint64_t> set = getAll();
                    uint64_t cbid = getInsertingNeighbour(params, *oldValue, id, set);
                    uint64_t bid = getInsertingNeighbour(params, *modifiedValue, id, set);
                    
                    if (cbid != bid) {
                        W::Address dest = oItr->first + W::Address({u"moveElement"});
                        W::Vocabulary* vc = new W::Vocabulary();
                        vc->insert(u"id", new W::Uint64(id));
                        if (id != 0) {
                            vc->insert(u"before", new W::Uint64(bid));
                        }
                        send(vc, dest, sItr->first);
                    }
                }
            }
        }
    }
}

void M::ICatalogue::h_subscribeMember(const W::Event& ev)
{
    const W::Address& addr = ev.getDestination();
    W::Address lastHops = addr << address.length();
    
    if (lastHops.length() == 2 && (lastHops.ends(W::Address{u"subscribe"}) || lastHops.ends(W::Address{u"get"}))) {
        W::Vocabulary* record;
        try {
            uint64_t id = lastHops.front().toUint64();
            record = getElement(id);
            if (lastHops.ends(W::Address{u"subscribe"})) {
                M::Vocabulary* modelRecord = new M::Vocabulary(record, address + lastHops >> 1);
                addModel(modelRecord);
                activeChildren.insert(std::make_pair(id, modelRecord));
                modelRecord->_h_subscribe(ev);
            } else {
                W::Vocabulary* vc = new W::Vocabulary;
                vc->insert(u"data", record);
                
                fakeResponse(vc, W::Address({u"get"}), addr >> 1, ev);
            }
        } catch(int err) {
            if (err == 3) {
                emit serviceMessage(QString("An attempt to create and subscribe record model in catalogue, but it is not found. Event: ") + ev.toString().c_str());
            } else {
                throw err;
            }
        } catch (const std::invalid_argument& err) {
            emit serviceMessage(QString("Strange event in custom handler of catalogue ") + ev.toString().c_str());
        }
    } else {
        emit serviceMessage(QString("Strange event in custom handler of catalogue ") + ev.toString().c_str());
    }
    
}

bool M::ICatalogue::match(const W::Vocabulary& value, const W::Vocabulary& filter)
{
    bool m = true;
    W::Vector keys = filter.keys();
    for (int i = 0; i < keys.length(); ++i) {
        const W::String& key = static_cast<const W::String&>(keys.at(i));
        if (filter.at(key) != value.at(key)) {
            m = false;
            break;
        };
    }
    
    return m;
}

void M::ICatalogue::processAddElement(uint64_t id, const W::Vocabulary& value, SMap::const_iterator subscriberIterator, uint64_t socketId)
{
    const W::Address& addr = subscriberIterator->first;
    const W::Vocabulary& params = subscriberIterator->second;
    const W::Vocabulary& filter = static_cast<const W::Vocabulary&>(params.at(u"filter"));
    
    std::set<uint64_t> set = find(filter);
    std::set<uint64_t>::const_iterator idItr = set.find(id);
    if (idItr != set.end()) {                                           //to make sure if subscriber cares
        uint64_t bid = getInsertingNeighbour(params, value, id, set);
        
        W::Address dest = addr + W::Address({u"addElement"});
        W::Vocabulary* vc = new W::Vocabulary();
        vc->insert(u"id", new W::Uint64(id));
        if (id != 0) {
            vc->insert(u"before", new W::Uint64(bid));
        }
        send(vc, dest, socketId);
    }
}

uint64_t M::ICatalogue::getInsertingNeighbour(const W::Vocabulary& params, const W::Vocabulary& record, uint64_t id, const std::set<uint64_t>& allowed) const
{
    uint64_t bid;
    if (params.has(u"sorting")) {
        if (allowed.empty()) {
            bid = 0;
        } else {
            const W::Vocabulary& sorting = static_cast<const W::Vocabulary&>(params.at(u"sorting"));
            const W::String& field = static_cast<const W::String&>(sorting.at(u"field"));
            bool ascending = static_cast<const W::Boolean&>(sorting.at(u"ascending"));
            
            uint64_t foundId = id;
            do {
                if (ascending) {
                    foundId = indexes.at(field)->getNext(foundId, record.at(field));
                } else {
                    foundId = indexes.at(field)->getPrev(foundId, record.at(field));
                }
            } while (allowed.find(foundId) == allowed.end() || foundId != 0);   //to make sure, that id folowing the inserting also present in the 
            bid = foundId;                                                      //subscribers filter result
        }
    } else {
        std::set<uint64_t>::const_iterator idItr = allowed.find(id);
        if (idItr == allowed.end()) {
            bid = 0;
        } else {
            ++idItr;
            if (idItr == allowed.end()) {
                bid = 0;
            } else {
                bid = *idItr;
            }
        }
    }
    
    return bid;
}

M::ICatalogue::AbstractIndex::TypeError::TypeError(const std::string& name, const std::string& method, W::Object::objectType myType, W::Object::objectType valueType): 
    Utils::Exception(),
    name(name),
    method(method),
    myType(myType),
    valueType(valueType)
{}

std::string M::ICatalogue::AbstractIndex::TypeError::getMessage() const
{
    std::string msg = "An attempt to call Catalogue Index of ";
    msg += name;
    msg += " method \"";
    msg += method;
    msg += "\" with value type of ";
    msg += W::Object::getTypeName(valueType);
    msg += " but this index values supposed to have type ";
    msg += W::Object::getTypeName(myType);
    
    return msg;
}
