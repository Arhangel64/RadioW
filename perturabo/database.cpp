#include "database.h"

#include <sys/stat.h>
#include <sys/types.h>

#include <wType/bytearray.h>
#include <algorithm>

Database::Database(const W::String& dbName, QObject* parent):
    M::List(W::Address({dbName}), parent),
    subscribeMember(W::Handler::create(W::Address({}), this, &Database::_h_subscribeMember)),
    name(dbName),
    opened(false),
    environment(lmdb::env::create()),
    dbi(0),
    indexes(),
    lastIndex(0)
{
}

Database::~Database()
{
    IndexMap::const_iterator itr = indexes.begin();
    IndexMap::const_iterator end = indexes.end();
    
    for (; itr != end; ++itr) {
        delete itr->second;
    }
    
    delete subscribeMember;
}

void Database::open()
{
    if (!opened) {
        checkDirAndOpenEnvironment();
        index();
        opened = true;
    }
}

const std::set<uint64_t> & Database::find(const W::String& indexName, const W::Object& value) const
{
    IndexMap::const_iterator itr = indexes.find(indexName);
    if (itr == indexes.end()) {
        throw 4;
    }
    
    return itr->second->find(value);
}

std::set<uint64_t> Database::find(const W::Vocabulary& value) const
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


void Database::addIndex(const W::String& fieldName, W::Object::objectType fieldType)
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
    
    if (opened) {
        lmdb::txn rtxn = lmdb::txn::begin(environment, nullptr, MDB_RDONLY);
        lmdb::cursor cursor = lmdb::cursor::open(rtxn, dbi);
        lmdb::val key;
        lmdb::val value;
        while (cursor.get(key, value, MDB_NEXT)) {
            uint64_t iKey = *((uint64_t*) key.data());
            W::ByteArray ba;
            char* bdata = value.data();
            for (std::size_t i = 0; i < value.size(); ++i) {
                ba.push(bdata[i]);
            }
            W::Vocabulary* wVal = static_cast<W::Vocabulary*>(W::Object::fromByteArray(ba));
            
            IndexMap::const_iterator itr = indexes.find(fieldName);
            itr->second->add(wVal->at(itr->first), iKey);
            
            delete wVal;
        }
        cursor.close();
        rtxn.abort();
    }
}

uint64_t Database::addRecord(const W::Vocabulary& record)
{
    IndexMap::const_iterator itr = indexes.begin();
    IndexMap::const_iterator end = indexes.end();
    
    ++lastIndex;
    
    for (; itr != end; ++itr) {
        itr->second->add(record.at(itr->first), lastIndex);
    }
    
    W::ByteArray ba;
    ba << record;
    int length = ba.size();
    uint8_t data[length];
    for (int i = 0; i < length; ++i) {
        data[i] = ba.pop();
    }
    lmdb::val key((uint8_t*) &lastIndex, 8);
    lmdb::val value(data, length);
    lmdb::txn wTrans = lmdb::txn::begin(environment);
    dbi.put(wTrans, key, value);
    wTrans.commit();
    
    push(W::Uint64(lastIndex));
    
    return lastIndex;
}


void Database::checkDirAndOpenEnvironment()
{
    int state1 = mkdir("./db", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (state1 != 0 && errno != EEXIST) {
        emit serviceMessage("Failed to create a root database folder");
        throw 1;
    }
    
    W::String path("./db/");
    path += name;
    
    int state2 = mkdir(path.toString().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (state2 != 0 && errno != EEXIST) {
        emit serviceMessage(QString("Failed to create ") + name.toString().c_str() + " database folder");
        throw 1;
    }
    
    environment.set_max_dbs(10);
    environment.open(path.toString().c_str(), 0, 0664);
    
    lmdb::txn wTrans = lmdb::txn::begin(environment);
    dbi = lmdb::dbi::open(wTrans, "main", MDB_CREATE | MDB_INTEGERKEY);
    wTrans.commit();
}

void Database::index()
{
    lmdb::txn rtxn = lmdb::txn::begin(environment, nullptr, MDB_RDONLY);
    lmdb::cursor cursor = lmdb::cursor::open(rtxn, dbi);
    lmdb::val key;
    lmdb::val value;
    while (cursor.get(key, value, MDB_NEXT)) {
        uint64_t iKey = *((uint64_t*) key.data());
        W::ByteArray ba;
        char* bdata = value.data();
        for (std::size_t i = 0; i < value.size(); ++i) {
            ba.push(bdata[i]);
        }
        W::Vocabulary* wVal = static_cast<W::Vocabulary*>(W::Object::fromByteArray(ba));
        
        IndexMap::const_iterator itr = indexes.begin();
        IndexMap::const_iterator end = indexes.end();
        
        for (; itr != end; ++itr) {
            itr->second->add(wVal->at(itr->first), iKey);
        }
        
        push(W::Uint64(iKey));
        if (iKey > lastIndex) {
            lastIndex = iKey;
        }
        
        delete wVal;
    }
    cursor.close();
    rtxn.abort();
}

W::Vocabulary* Database::getRecord(uint64_t id)
{
    lmdb::txn rtxn = lmdb::txn::begin(environment, nullptr, MDB_RDONLY);
    lmdb::val key((uint8_t*) &id, 8);
    lmdb::val value;
    
    if (dbi.get(rtxn, key, value)) {
        W::ByteArray ba;
        char* bdata = value.data();
        for (std::size_t i = 0; i < value.size(); ++i) {
            ba.push(bdata[i]);
        }
        
        rtxn.abort();
        
        W::Vocabulary* wVal = static_cast<W::Vocabulary*>(W::Object::fromByteArray(ba));
        
        return wVal;
    } else {
        rtxn.abort();
        throw 3;
    }
}

void Database::h_subscribeMember(const W::Event& ev)
{
    const W::Address& addr = ev.getDestination();
    W::Address lastHops = addr << address.size();
    
    if (lastHops.size() == 2 && (lastHops.ends(W::Address{u"subscribe"}) || lastHops.ends(W::Address{u"get"}))) {
        W::Vocabulary* record;
        try {
            record = getRecord(lastHops.front().toUint64());
            M::Vocabulary* modelRecord = new M::Vocabulary(record, address + lastHops >> 1);
            if (lastHops.ends(W::Address{u"subscribe"})) {
                addModel(modelRecord);
                //passToHandler(ev);  //not sure about this. May be it's better to make this class a friend of M::Model or M::Vocabulary, so i can call handler directly?
                modelRecord->_h_subscribe(ev);
            } else {
                W::Vocabulary* vc = new W::Vocabulary;
                vc->insert(u"data", record);
    
                fakeResponse(vc, W::Address({u"get"}), addr >> 1, ev);
            }
        } catch(int err) {
            if (err == 3) {
                emit serviceMessage(QString("An attempt to create and subscribe record model in database, but it is not found. Event: ") + ev.toString().c_str());
            } else {
                throw err;
            }
        }
    } else {
        emit serviceMessage(QString("Strange event in custom handler of database ") + ev.toString().c_str());
    }
    
}

void Database::addModel(M::Model* model)
{
    connect(model, SIGNAL(subscribersCountChange(uint64_t)), this, SLOT(onChildSubscribersCountChange(uint64_t)));
    
    M::List::addModel(model);
}

void Database::removeModel(M::Model* model)
{
    disconnect(model, SIGNAL(subscribersCountChange(uint64_t)), this, SLOT(onChildSubscribersCountChange(uint64_t)));
    
    M::List::removeModel(model);
}


void Database::onChildSubscribersCountChange(uint64_t count)
{
    if (count == 0) {
        M::Model* model = static_cast<M::Model*>(sender());
        
        removeModel(model);
        emit serviceMessage(QString("Unregistered model ") + model->getAddress().toString().c_str() + " because there no subscribers left");
        
        model->deleteLater();
    }
}

