#ifndef CATALOGUE_H
#define CATALOGUE_H

#include "icatalogue.h"

namespace M {
    
    class Catalogue : public ICatalogue {
    public:
        Catalogue(const W::Address p_address, QObject* parent = 0);
        ~Catalogue();
        
        uint64_t addElement(const W::Vocabulary & record) override;
        void removeElement(uint64_t id) override;
        void clear() override;
        W::Vocabulary* getElement(uint64_t id) override;
        void modifyElement(uint64_t id, const W::Vocabulary & newValue) override;
        uint64_t size() const override;
        
    protected:
        std::set<uint64_t> getAll() const override;
        
    private:
        typedef std::map<uint64_t, W::Vocabulary*> Data;
        Data data;
    };
}

#endif // CATALOGUE_H
