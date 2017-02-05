#include "database.h"

#include <sys/stat.h>
#include <sys/types.h>

#include <wType/bytearray.h>

Database::Database(const W::String& dbName, QObject* parent):
    M::Vocabulary(W::Address({dbName}), parent),
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
        
        if (iKey > lastIndex) {
            lastIndex = iKey;
        }
        
        delete wVal;
    }
    cursor.close();
    rtxn.abort();
}
