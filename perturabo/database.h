#ifndef DATABASE_H
#define DATABASE_H

#include <utils/lmdb++.h>

#include <map>
#include <set>

#include <wModel/list.h>
#include <wModel/vocabulary.h>

#include <wType/string.h>
#include <wType/address.h>

class Database: public M::List
{
    Q_OBJECT
    class AbstractIndex;
public:
    Database(const W::String& dbName, QObject* parent = 0);
    ~Database();
    
    void open();
    void addIndex(const W::String& fieldName, W::Object::objectType fieldType);
    uint64_t addRecord(const W::Vocabulary& record);
    W::Vocabulary* getRecord(uint64_t id);
    const std::set<uint64_t>& find(const W::String& indexName, const W::Object& value) const;
    std::set<uint64_t> find(const W::Vocabulary& value) const;
    
    W::Handler* subscribeMember;
    handler(subscribeMember);
    
    void addModel(M::Model* model);
    void removeModel(M::Model* model);
    
private:
    void checkDirAndOpenEnvironment();
    void index();
    
private slots:
    void onChildSubscribersCountChange(uint64_t count);
    
private:
    typedef std::map<W::String, AbstractIndex*> IndexMap;
    W::String name;
    bool opened;
    lmdb::env environment;
    lmdb::dbi dbi;
    IndexMap indexes;
    uint64_t lastIndex;
    
    
    
    
    
    
    class AbstractIndex {
    public:
        AbstractIndex(W::Object::objectType vt): valueType(vt), empty() {}
        virtual ~AbstractIndex() {}
        
        virtual const std::set<uint64_t>& find(const W::Object& value) const = 0;
        virtual void add(const W::Object& value, uint64_t id) = 0;
        
        W::Object::objectType valueType;
        
    protected:
        std::set<uint64_t> empty;
    };
    
    template <class T> 
    class Index : public AbstractIndex {
    public:
        Index();
        ~Index();
        
        virtual const std::set<uint64_t>& find(const W::Object& value) const override;
        void add(const W::Object & value, uint64_t id) override;
        
    private:
        typedef std::map<T, std::set<uint64_t>> Map;
        
        Map values;
        
    };
};


template<class T> 
Database::Index<T>::Index():
    Database::AbstractIndex(T::type),
    values()
{
}

template<class T> 
Database::Index<T>::~Index()
{
}

template<class T> 
const std::set<uint64_t> & Database::Index<T>::find(const W::Object& value) const
{
    if (value.getType() != valueType) {
        throw 1;
    }
        
    const T& val = static_cast<const T&>(value);
    typename std::map<T, std::set<uint64_t>>::const_iterator itr = values.find(val);
    
    if (itr == values.end()) {
        return empty;
    } else {
        return itr->second;
    }
}

template<class T> 
void Database::Index<T>::add(const W::Object& value, uint64_t id)
{
    if (value.getType() != valueType) {
        throw 1;
    }
    
    const T& val = static_cast<const T&>(value);
    typename std::map<T, std::set<uint64_t>>::iterator itr = values.find(val);
    if (itr == values.end()) {
        itr = values.insert(std::make_pair(val, std::set<uint64_t>())).first;
    }
    itr->second.insert(id);
}

#endif // DATABASE_H
