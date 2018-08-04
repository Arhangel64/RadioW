#include "resourcecache.h"

#include <sys/stat.h>
#include <sys/types.h>

#include <fstream>

ResourceCache::ResourceCache(const W::String& dbName, QObject* parent):
    M::Model(W::Address{dbName}, parent),
    subscribeMember(W::Handler::create(W::Address({}), this, &ResourceCache::_h_subscribeMember)),
    name(dbName),
    opened(false),
    environment(lmdb::env::create()),
    dbi(0),
    elements(),
    lastIndex(0)
{
    W::Handler* get = W::Handler::create(address + W::Address({u"get"}), this, &ResourceCache::_h_get);
    addHandler(get);
}

ResourceCache::~ResourceCache()
{
    delete subscribeMember;
}

void ResourceCache::open()
{
    if (!opened) {
        checkDirAndOpenEnvironment();
        
        lmdb::txn rtxn = lmdb::txn::begin(environment, nullptr, MDB_RDONLY);
        lmdb::cursor cursor = lmdb::cursor::open(rtxn, dbi);
        lmdb::val key;
        lmdb::val value;
        while (cursor.get(key, value, MDB_NEXT)) {
            uint64_t iKey = *((uint64_t*) key.data());
            
            elements.insert(iKey);
            if (iKey > lastIndex) {
                lastIndex = iKey;
            }
        }
        cursor.close();
        rtxn.abort();
        
        opened = true;
        emit countChange(elements.size());
    }
}

void ResourceCache::checkDirAndOpenEnvironment()
{
    int state1 = mkdir("./dbMedia", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (state1 != 0 && errno != EEXIST) {
        emit serviceMessage("Failed to create a root database folder");
        throw 1;
    }
    
    W::String path("./dbMedia/");
    path += name;
    
    int state2 = mkdir(path.toString().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (state2 != 0 && errno != EEXIST) {
        emit serviceMessage(QString("Failed to create ") + name.toString().c_str() + " database folder");
        throw 1;
    }
    
    environment.set_mapsize(1UL * 1024UL * 1024UL * 1024UL);
    environment.set_max_dbs(10);
    environment.open(path.toString().c_str(), 0, 0664);
    
    lmdb::txn wTrans = lmdb::txn::begin(environment);
    dbi = lmdb::dbi::open(wTrans, "main", MDB_CREATE | MDB_INTEGERKEY);
    wTrans.commit();
}

uint64_t ResourceCache::addResource(const W::String& path)
{
    uint64_t id = ++lastIndex;
    
    elements.insert(id);
    
    W::ByteArray ba(path.size() + 1);
    ba.push8(path.getType());
    path.serialize(ba);
    
    lmdb::val key((uint8_t*) &id, 8);
    lmdb::val value(ba.getData(), ba.size());
    lmdb::txn wTrans = lmdb::txn::begin(environment);
    dbi.put(wTrans, key, value);
    wTrans.commit();
    
    W::Vocabulary* vc = new W::Vocabulary();
    vc->insert(u"id", new W::Uint64(id));
    
    broadcast(vc, W::Address({u"addElement"}));
    emit countChange(elements.size());
    
    return id;
}

void ResourceCache::removeResource(uint64_t id)
{
    if (!opened) {
        throw ClosedDB("removeResource");
    }
    
    lmdb::txn transaction = lmdb::txn::begin(environment);
    lmdb::val key((uint8_t*) &id, 8);
    dbi.del(transaction, key);
    transaction.commit();
    elements.erase(id);
    
    W::Vocabulary* vc = new W::Vocabulary();
    vc->insert(u"id", new W::Uint64(id));
    
    broadcast(vc, W::Address({u"removeElement"}));
    emit countChange(elements.size());
    
    //TODO not sure, may be it's better to also destroy resource model?
}

void ResourceCache::clear()
{
    if (!opened) {
        throw new ClosedDB("clear");
    }
    
    lmdb::txn transaction = lmdb::txn::begin(environment);
    dbi.drop(transaction);
    transaction.commit();
    
    elements.clear();
    lastIndex = 0;
    
    W::Vocabulary* vc = new W::Vocabulary();
    
    broadcast(vc, W::Address({u"clear"}));
    emit countChange(elements.size());
}

void ResourceCache::h_get(const W::Event& ev)
{
    W::Vector* order = new W::Vector();
    std::set<uint64_t>::const_iterator itr = elements.begin();
    std::set<uint64_t>::const_iterator end = elements.end();
    
    for (; itr != end; ++itr) {
        order->push(W::Uint64(*itr));
    }
    
    W::Vocabulary* rvc = new W::Vocabulary;
    rvc->insert(u"data", order);
    
    response(rvc, W::Address({u"get"}), ev);
}


M::Model::ModelType ResourceCache::getType() const
{
    return type;
}

void ResourceCache::set(W::Object* value)
{
    set(*value);
}

void ResourceCache::set(const W::Object& value)
{
    throw 14;   //what do you expect here? not implemented, and not sure it ever would be
}

void ResourceCache::h_subscribe(const W::Event& ev)
{
    M::Model::h_subscribe(ev);
    
    h_get(ev);
}

W::String * ResourceCache::getElement(uint64_t id) const
{
    lmdb::txn rtxn = lmdb::txn::begin(environment, nullptr, MDB_RDONLY);
    lmdb::val key((uint8_t*) &id, 8);
    lmdb::val value;
    
    if (lmdb::dbi_get(rtxn, dbi.handle(), key, value)) {
        W::ByteArray ba(value.size());
        ba.fill(value.data(), value.size());
        
        W::String* wVal = static_cast<W::String*>(W::Object::fromByteArray(ba));
        rtxn.abort();
        
        return wVal;
    } else {
        rtxn.abort();
        throw 3;
    }
}


void ResourceCache::h_subscribeMember(const W::Event& ev)
{
    const W::Address& addr = ev.getDestination();
    W::Address lastHops = addr << address.length();
    
    if (lastHops.length() == 2 && (lastHops.ends(W::Address{u"subscribe"}) 
        || lastHops.ends(W::Address{u"get"}) 
        || lastHops.ends(W::Address{u"getAdditional"}))
    ) {
        W::String* record;
        try {
            record = getElement(lastHops.front().toUint64());
            M::File* modelRecord = M::File::create(readFile(*record), address + lastHops >> 1);
            delete record;
            addModel(modelRecord);
            passToLocalHandler(ev);
        } catch (int err) {
            if (err == 3) {
                emit serviceMessage(QString("An attempt to create and subscribe record model in resourcecache, but it is not found. Event: ") + ev.toString().c_str());
            } else if (err == 10) {
                serviceMessage(QString("Can't open file ") + record->toString().c_str());
                delete record;
            } else {
                throw err;
            }
        } catch (const std::invalid_argument& err) {
            emit serviceMessage(QString("Strange event in custom handler of resourcecache ") + ev.toString().c_str());
        }
    } else {
        emit serviceMessage(QString("Strange event in custom handler of resourcecache ") + ev.toString().c_str());
    }
}


W::Blob * ResourceCache::readFile(const W::String& path) const
{
    std::ifstream file (path.toString(), std::ios::in|std::ios::binary|std::ios::ate);
    
    if (file.is_open()) {
        char * memblock;
        uint32_t size;
        size = file.tellg();
        file.seekg(0, std::ios::beg);
        memblock = new char[size];
        file.read(memblock, size);
        file.close();
        
        return new W::Blob(size, memblock);
    } else {
        throw 10;   //TODO
    }
}

std::set<uint64_t> ResourceCache::getAllIdentificators() const
{
    if (!opened) {
        throw new ClosedDB("getAllIdentificators");
    }
    
    return elements;
}

W::String ResourceCache::getPath(uint64_t id) const
{
    return *(getElement(id));
}

bool ResourceCache::has(uint64_t id) const
{
    if (!opened) {
        throw new ClosedDB("has");
    }
    
    return elements.find(id) != elements.end();
}
