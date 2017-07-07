#include "icatalogue.h"

M::ICatalogue::ICatalogue(const W::Address p_address, QObject* parent):
    M::Model(p_address, parent),
    indexes(),
    lastIndex(0)
{
    W::Handler* get = W::Handler::create(address + W::Address({u"get"}), this, &M::ICatalogue::_h_get);
    addHandler(get);
}

M::ICatalogue::~ICatalogue()
{
    IndexMap::const_iterator itr = indexes.begin();
    IndexMap::const_iterator end = indexes.end();
    
    for (; itr != end; ++itr) {
        delete itr->second;
    }
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
    int size = keys.size();
    
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
        
        if (result.size() == 0) {
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
                const W::Vocabulary& filter = static_cast<const W::Vocabulary&>(params.at(u"filter"));
                std::set<uint64_t> set = find(filter);
                std::set<uint64_t>::const_iterator idItr = set.find(lastIndex);
                if (idItr != set.end()) {                                           //to make sure if subscriber cares
                    uint64_t id;
                    if (params.has(u"sorting")) {
                        const W::Vocabulary& sorting = static_cast<const W::Vocabulary&>(params.at(u"sorting"));
                        const W::String& field = static_cast<const W::String&>(sorting.at(u"field"));
                        bool ascending = static_cast<const W::Boolean&>(sorting.at(u"ascending"));
                        
                        uint64_t foundId = lastIndex;
                        do {
                            if (ascending) {
                                foundId = indexes.at(field)->getNext(foundId, record.at(field));
                            } else {
                                foundId = indexes.at(field)->getPrev(foundId, record.at(field));
                            }
                        } while (set.find(foundId) == set.end() || foundId != 0);   //to make sure, that id folowing the inserting also present in the 
                        id = foundId;                                               //subscribers filter result
                    } else {
                        ++idItr;
                        if (idItr == set.end()) {
                            id = 0;
                        } else {
                            id = *idItr;
                        }
                    }
                    
                    W::Address dest = oItr->first + W::Address({u"addElement"});
                    W::Vocabulary* vc = new W::Vocabulary();
                    vc->insert(u"id", new W::Uint64(lastIndex));
                    if (id != 0) {
                        vc->insert(u"before", new W::Uint64(id));
                    }
                    send(vc, dest, sItr->first);
                }
            } else {
                uint64_t id;
                if (params.has(u"sorting")) {
                    const W::Vocabulary& sorting = static_cast<const W::Vocabulary&>(params.at(u"sorting"));
                    const W::String& field = static_cast<const W::String&>(sorting.at(u"field"));
                    bool ascending = static_cast<const W::Boolean&>(sorting.at(u"ascending"));
                    
                    if (ascending) {
                        id = indexes.at(field)->getNext(lastIndex, record.at(field));
                    } else {
                        id = indexes.at(field)->getPrev(lastIndex, record.at(field));
                    }
                } else {
                    std::set<uint64_t> set = getAll();
                    std::set<uint64_t>::const_iterator idItr = set.find(lastIndex);
                    ++idItr;
                    if (idItr == set.end()) {
                        id = 0;
                    } else {
                        id = *idItr;
                    }
                }
                
                W::Address dest = oItr->first + W::Address({u"addElement"});
                W::Vocabulary* vc = new W::Vocabulary();
                vc->insert(u"id", new W::Uint64(lastIndex));
                if (id != 0) {
                    vc->insert(u"before", new W::Uint64(id));
                }
                send(vc, dest, sItr->first);
            }
        }
    }
    
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
    
    W::Vector order;
    if (params.has(u"sorting")) {
        const W::Vocabulary& sorting = static_cast<const W::Vocabulary&>(params.at(u"sorting"));
        const W::String& field = static_cast<const W::String&>(sorting.at(u"field"));
        bool ascending = static_cast<const W::Boolean&>(sorting.at(u"ascending"));
        order = indexes.at(field)->sort(set, ascending);
    } else {
        std::set<uint64_t>::const_iterator itr = set.begin();
        std::set<uint64_t>::const_iterator end = set.end();
        
        for (; itr != end; ++itr) {
            order.push(W::Uint64(*itr));
        }
    }
    
    W::Vocabulary* rvc = new W::Vocabulary;
    rvc->insert(u"data", order);            //TODO copy another time? need to do better;
    
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
