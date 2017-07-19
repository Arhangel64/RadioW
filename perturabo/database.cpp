#include "database.h"

#include <sys/stat.h>
#include <sys/types.h>

#include <wType/bytearray.h>
#include <algorithm>

Database::Database(const W::String& dbName, QObject* parent):
    M::ICatalogue(W::Address({dbName}), parent),
    subscribeMember(W::Handler::create(W::Address({}), this, &Database::_h_subscribeMember)),
    name(dbName),
    opened(false),
    environment(lmdb::env::create()),
    dbi(0),
    elements()
{
}

Database::~Database()
{
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

void Database::addIndex(const W::String& fieldName, W::Object::objectType fieldType)
{
    ICatalogue::addIndex(fieldName, fieldType);
    
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

uint64_t Database::addElement(const W::Vocabulary& record)
{
    uint64_t id = ICatalogue::addElement(record);
    
    elements.insert(id);
    
    W::ByteArray ba;
    ba << record;
    int length = ba.size();
    uint8_t data[length];
    for (int i = 0; i < length; ++i) {
        data[i] = ba.pop();
    }
    lmdb::val key((uint8_t*) &id, 8);
    lmdb::val value(data, length);
    lmdb::txn wTrans = lmdb::txn::begin(environment);
    dbi.put(wTrans, key, value);
    wTrans.commit();
    
    return id;
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
        ICatalogue::addElement(*wVal);
        
        elements.insert(iKey);
        delete wVal;
    }
    cursor.close();
    rtxn.abort();
}

W::Vocabulary* Database::getElement(uint64_t id)
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
            record = getElement(lastHops.front().toUint64());
            M::Vocabulary* modelRecord = new M::Vocabulary(record, address + lastHops >> 1);
            if (lastHops.ends(W::Address{u"subscribe"})) {
                addModel(modelRecord);
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
        } catch (const std::invalid_argument& err) {
            emit serviceMessage(QString("Strange event in custom handler of database ") + ev.toString().c_str());
        }
    } else {
        emit serviceMessage(QString("Strange event in custom handler of database ") + ev.toString().c_str());
    }
    
}

std::set<uint64_t> Database::getAll()
{
    return elements;
}

void Database::removeElement(uint64_t id)
{
    if (!opened) {
        throw 6;
    }
    ICatalogue::removeElement(id);
    
    lmdb::txn rtxn = lmdb::txn::begin(environment, nullptr, MDB_RDONLY);
    lmdb::val key((uint8_t*) &id, 8);
    dbi.del(rtxn, key);
    elements.erase(id);
}


void Database::addModel(M::Model* model)
{
    connect(model, SIGNAL(subscribersCountChange(uint64_t)), this, SLOT(onChildSubscribersCountChange(uint64_t)));
    
    M::ICatalogue::addModel(model);
}

void Database::removeModel(M::Model* model)
{
    disconnect(model, SIGNAL(subscribersCountChange(uint64_t)), this, SLOT(onChildSubscribersCountChange(uint64_t)));
    
    M::ICatalogue::removeModel(model);
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

