#ifndef RESOURCECACHE_H
#define RESOURCECACHE_H

#include <set>
#include <map>

#include "lmdb++.h"

#include <wModel/model.h>
#include <wModel/file/file.h>
#include <wType/address.h>
#include <wType/string.h>
#include <wType/bytearray.h>
#include <wType/uint64.h>
#include <wType/blob.h>
#include <utils/exception.h>


class ResourceCache : public M::Model
{
    Q_OBJECT
public:
    ResourceCache(const W::String& dbName, QObject* parent = 0);
    ~ResourceCache();
    
    void open();
    uint64_t addResource(const W::String& path);
    void removeResource(uint64_t id);
    void clear();
    bool has(uint64_t id) const;
    W::String getPath(uint64_t id) const;
    std::set<uint64_t> getAllIdentificators() const;
    
    W::Handler* subscribeMember;
    handler(subscribeMember);
    
    M::Model::ModelType getType() const override;
    static const M::Model::ModelType type = resourceCache;
    void set(const W::Object & value) override;
    void set(W::Object * value) override;

signals:
    void countChange(uint64_t count);
    
protected:
    void h_subscribe(const W::Event & ev) override;
    
    handler(get);
    
    W::String* getElement(uint64_t id) const;
    
public:
    const W::String name;
    
private:
    bool opened;
    lmdb::env environment;
    lmdb::dbi dbi;
    std::set<uint64_t> elements;
    uint64_t lastIndex;
    
    void checkDirAndOpenEnvironment();
    W::Blob* readFile(const W::String& path) const;
    
    class ClosedDB: 
    public Utils::Exception
    {
    public:
        ClosedDB(const std::string& p_op):Exception(), operation(p_op){}
        
        std::string getMessage() const {
            std::string str = "An attempt to perform method ResourceCache::";
            str += operation;
            str += " but the database is not open";
            return str;
            
        }
        
    private:
        std::string operation;
    };
};

#endif // RESOURCECACHE_H
