#ifndef ICATALOGUE_H
#define ICATALOGUE_H

#include "model.h"

#include <set>
#include <map>

#include <wModel/vocabulary.h>
#include <utils/exception.h>

namespace M {
    class ICatalogue : public M::Model
    {
        Q_OBJECT
    protected:
        class AbstractIndex;
    public:
        ICatalogue(const W::Address p_address, QObject* parent = 0);
        ~ICatalogue();
        
        virtual uint64_t addElement(const W::Vocabulary& record);
        virtual void removeElement(uint64_t id);
        virtual W::Vocabulary* getElement(uint64_t id) = 0;
        virtual void modifyElement(uint64_t id, const W::Vocabulary& newValue) = 0;
        virtual uint64_t size() const = 0;
        virtual void clear();
        
        virtual void addIndex(const W::String& fieldName, W::Object::objectType fieldType);
        const std::set<uint64_t>& find(const W::String& indexName, const W::Object& value) const;
        std::set<uint64_t> find(const W::Vocabulary& value) const;
        
        M::Model::ModelType getType() const override;
        static const M::Model::ModelType type = catalogue;
        void set(const W::Object & value) override;
        void set(W::Object * value) override;
        
        W::Handler* subscribeMember;
        handler(subscribeMember);
        
        static bool match(const W::Vocabulary& value, const W::Vocabulary& filter);
        static const std::set<uint64_t> empty;
        
    signals:
        void countChange(uint64_t count);
        
    protected:
        virtual std::set<uint64_t> getAll() const = 0;
        void h_subscribe(const W::Event & ev) override;
        
        handler(get);
        handler(add);
        handler(update);
        
        typedef std::map<W::String, AbstractIndex*> IndexMap;
        IndexMap indexes;
        
    private:
        uint64_t lastIndex;
        std::map<uint64_t, M::Vocabulary*> activeChildren;
        
        void processAddElement(uint64_t id, const W::Vocabulary& value, SMap::const_iterator subscriberIterator, uint64_t socketId);
        uint64_t getInsertingNeighbour(const W::Vocabulary& params, const W::Vocabulary& record, uint64_t id, const std::set<uint64_t>& allowed = empty) const;
        
    protected:
        class AbstractIndex {
        public:
            AbstractIndex(W::Object::objectType vt): valueType(vt) {}
            virtual ~AbstractIndex() {}
            
            virtual const std::set<uint64_t>& find(const W::Object& value) const = 0;
            virtual void add(const W::Object& value, uint64_t id) = 0;
            virtual void remove(const W::Object & value, uint64_t id) = 0;
            virtual void clear() = 0;
            virtual W::Vector sort(const std::set<uint64_t>& set, bool ascending) = 0;
            virtual uint64_t getNext(uint64_t id, const W::Object& value) = 0;
            virtual uint64_t getPrev(uint64_t id, const W::Object& value) = 0;
            
            W::Object::objectType valueType;
            
        protected:
            class TypeError : public Utils::Exception {
            public:
                TypeError(const std::string& name, const std::string& method, W::Object::objectType myType, W::Object::objectType valueType);
                
                std::string getMessage() const;
                
            private:
                std::string name;
                std::string method;
                W::Object::objectType myType;
                W::Object::objectType valueType;
            };
        };
        
        template <class T> 
        class Index : public AbstractIndex {
        public:
            Index();
            ~Index();
            
            const std::set<uint64_t>& find(const W::Object& value) const override;
            void add(const W::Object & value, uint64_t id) override;
            void remove(const W::Object & value, uint64_t id) override;
            void clear() override;
            W::Vector sort(const std::set<uint64_t> & set, bool ascending) override;
            uint64_t getNext(uint64_t id, const W::Object& value) override;
            uint64_t getPrev(uint64_t id, const W::Object& value) override;
            
        private:
            typedef std::map<T, std::set<uint64_t>> Map;
            
            Map values;
            
        };
    };
    
    
    
    template<class T> 
    ICatalogue::Index<T>::Index():
        ICatalogue::AbstractIndex(T::type),
        values()
    {
    }

    template<class T> 
    ICatalogue::Index<T>::~Index()
    {
    }

    template<class T> 
    const std::set<uint64_t> & ICatalogue::Index<T>::find(const W::Object& value) const
    {
        if (value.getType() != valueType) {
            throw new TypeError("Unknown", "find", valueType, value.getType());             //todo replace that unknown stuff, find a way to provide index name
        }
            
        const T& val = static_cast<const T&>(value);
        typename std::map<T, std::set<uint64_t>>::const_iterator itr = values.find(val);
        
        if (itr == values.end()) {
            return ICatalogue::empty;
        } else {
            return itr->second;
        }
    }

    template<class T> 
    void ICatalogue::Index<T>::add(const W::Object& value, uint64_t id)
    {
        if (value.getType() != valueType) {
            throw new TypeError("Unknown", "add", valueType, value.getType());
        }
        
        const T& val = static_cast<const T&>(value);
        typename std::map<T, std::set<uint64_t>>::iterator itr = values.find(val);
        if (itr == values.end()) {
            itr = values.insert(std::make_pair(val, std::set<uint64_t>())).first;
        }
        itr->second.insert(id);
    }
    
    template<class T> 
    void ICatalogue::Index<T>::remove(const W::Object& value, uint64_t id) 
    {
        if (value.getType() != valueType) {
            throw new TypeError("Unknown", "remove", valueType, value.getType());
        }
        const T& val = static_cast<const T&>(value);
        typename std::map<T, std::set<uint64_t>>::iterator itr = values.find(val);
        if (itr != values.end()) {
            std::set<uint64_t>& set = itr->second;
            if (set.size() == 1) {
                values.erase(itr);
            } else {
                std::set<uint64_t>::const_iterator hint = set.find(id);
                set.erase(hint);
            }
        }
    }
    
    template<class T> 
    void ICatalogue::Index<T>::clear() 
    {
        values.clear();
    }
    
    template<class T> 
    W::Vector ICatalogue::Index<T>::sort(const std::set<uint64_t> & set, bool ascending)        //TODO this needs an optimization
    {
        W::Vector res;
        std::set<uint64_t>::const_iterator sEnd = set.end();
        uint64_t size = set.size();
        
        if (size == 0) {
            return res;
        } else if (size == 1) {
            res.push(W::Uint64(*(set.begin())));
            return res;
        }
        if (ascending) {
            typename std::map<T, std::set<uint64_t>>::const_iterator itr = values.begin();
            typename std::map<T, std::set<uint64_t>>::const_iterator end = values.end();
            
            for (; itr != end; ++itr) {
                if (size == res.size()) {
                    break;
                }
                const std::set<uint64_t>& chunk = itr->second;
                
                std::set<uint64_t>::const_iterator cItr = chunk.begin();
                std::set<uint64_t>::const_iterator cEnd = chunk.end();
                for (; cItr != cEnd; ++cItr) {
                    uint64_t id = *cItr;
                    if (set.find(id) != sEnd) {
                        res.push(W::Uint64(id));
                    }
                }
            }
        } else {
            typename std::map<T, std::set<uint64_t>>::reverse_iterator itr = values.rbegin();
            typename std::map<T, std::set<uint64_t>>::reverse_iterator end = values.rend();
            
            for (; itr != end; ++itr) {
                if (size == res.size()) {
                    break;
                }
                const std::set<uint64_t>& chunk = itr->second;
                
                std::set<uint64_t>::const_iterator cItr = chunk.begin();
                std::set<uint64_t>::const_iterator cEnd = chunk.end();
                for (; cItr != cEnd; ++cItr) {
                    uint64_t id = *cItr;
                    if (set.find(id) != sEnd) {
                        res.push(W::Uint64(id));
                    }
                }
            }
        }
        
        return res;
    }
    
    template<class T> 
    uint64_t ICatalogue::Index<T>::getNext(uint64_t id, const W::Object& value) 
    {
        if (value.getType() != valueType) {
            throw new TypeError("Unknown", "getNext", valueType, value.getType());
        }
        const T& val = static_cast<const T&>(value);
        typename std::map<T, std::set<uint64_t>>::iterator itr = values.find(val);
        if (itr == values.end()) {
            throw 2;    //this is not suppose to happen!
        }
        const std::set<uint64_t>& set = itr->second;
        std::set<uint64_t>::const_iterator sItr = set.find(id);
        if (sItr == set.end()) {
            throw 2;     //not suppose to happen!
        }
        ++sItr;
        if (sItr == set.end()) {
            ++itr;
            bool found = false;
            while (itr != values.end()) {
                if (itr->second.size() != 0) {
                    sItr = set.begin();
                    found = true;
                    break;
                }
                ++itr;
            }
            if (!found) {
                return 0;
            }
        }
        return *sItr;
    }
    
    template<class T> 
    uint64_t ICatalogue::Index<T>::getPrev(uint64_t id, const W::Object& value) 
    {
        if (value.getType() != valueType) {
            throw new TypeError("Unknown", "getPrev", valueType, value.getType());
        }
        const T& val = static_cast<const T&>(value);
        typename std::map<T, std::set<uint64_t>>::iterator itr = values.find(val);
        if (itr == values.end()) {
            throw 2;    //this is not suppose to happen!
        }
        const std::set<uint64_t>& set = itr->second;
        std::set<uint64_t>::const_iterator sItr = set.find(id);
        if (sItr == set.end()) {
            throw 2;     //not suppose to happen!
        }
        if (sItr == set.begin()) {
            bool found = false;
            while (itr != values.begin()) {
                --itr;
                if (itr->second.size() != 0) {
                    sItr = set.end();
                    --sItr;
                    break;
                }
            }
            if (!found) {
                return 0;
            }
        } else {
            --sItr;
        }
        return *sItr;
    }
}

#endif // ICATALOGUE_H
