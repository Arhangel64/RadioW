#ifndef DATABASE_H
#define DATABASE_H

#include <utils/lmdb++.h>

#include <map>
#include <set>

#include <wModel/icatalogue.h>
#include <wModel/vocabulary.h>

#include <wType/string.h>
#include <wType/address.h>

class Database: public M::ICatalogue
{
    Q_OBJECT
    class AbstractIndex;
public:
    Database(const W::String& dbName, QObject* parent = 0);
    ~Database();
    
    void open();
    
    uint64_t addElement(const W::Vocabulary& record) override;
    W::Vocabulary* getElement(uint64_t id) override;
    void removeElement(uint64_t id) override;
    
    void addIndex(const W::String& fieldName, W::Object::objectType fieldType) override;
    
    W::Handler* subscribeMember;
    handler(subscribeMember);
    
    void addModel(M::Model* model);
    void removeModel(M::Model* model);
    
protected:
    std::set<uint64_t> getAll() override;
    
private:
    void checkDirAndOpenEnvironment();
    void index();
    
private slots:
    void onChildSubscribersCountChange(uint64_t count);
    
private:
    W::String name;
    bool opened;
    lmdb::env environment;
    lmdb::dbi dbi;
    std::set<uint64_t> elements;
};

#endif // DATABASE_H
