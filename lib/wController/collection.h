#ifndef COLLECTION_H
#define COLLECTION_H

#include "catalogue.h"
#include "vocabulary.h"

#include <wType/address.h>

#include <map>
#include <set>

namespace C {
    
    /**
     * @todo write docs
     */
    class Collection : public C::Catalogue {
        Q_OBJECT
    public:
        Collection(const W::Address p_address, QObject* parent = 0);
        ~Collection();
        
        std::set<uint64_t> find(const W::String& field, const W::Object& value);
        const C::Vocabulary& get(uint64_t id) const;

    signals:
        void ready();       //emits when every VC received their data;
        
    protected:
        void addElement(const W::Uint64 & id, const W::Uint64 & before) override;
        void clearCatalogue() override;
        void removeElement(const W::Uint64 & id) override;
        
        void h_get(const W::Event & ev) override;
        void h_clear(const W::Event & ev) override;
        
    private:
        typedef std::map<uint64_t, C::Vocabulary*> Elements;
        Elements elements;
        std::set<C::Vocabulary*> waitingElements;
        bool hasData;
        
        void addChildVocabulary(const W::Uint64& id);
        
    private slots:
        void onChildVCData();
        
    };
    
}

#endif // COLLECTION_H
