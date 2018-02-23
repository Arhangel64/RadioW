#ifndef CATALOGUE_H
#define CATALOGUE_H

/**
 * @todo write docs
 */
#include "controller.h"

#include <wType/address.h>
#include <wType/string.h>
#include <wType/vocabulary.h>
#include <wType/boolean.h>

#include <wContainer/order.h>

namespace C {
    class Catalogue : public Controller {
        Q_OBJECT
    public:
        Catalogue(const W::Address p_address, QObject* parent);
        ~Catalogue();
        
        void setSorting(const W::String& field, bool ascending = true);
        void clearSorting();
        
        void addRemoteElement(const W::Vocabulary& element) const;
        void updateRemoteElement(const W::Uint64& id, const W::Vocabulary& newValue) const;
        
    signals:
        void addedElement(uint64_t id, uint64_t before = 0);
        void movedElement(uint64_t id, uint64_t before = 0);
        void removedElement(uint64_t id);
        void clear();
        void data();
        
    protected:
        handler(get)
        handler(addElement)
        handler(removeElement)
        handler(moveElement)
        handler(clear)
        
        virtual void addElement(const W::Uint64& id, const W::Uint64& before = W::Uint64(0));
        virtual void clearCatalogue();
        virtual void removeElement(const W::Uint64& id);
        virtual void getData();
        W::Vocabulary* createSubscriptionVC() const override;
        
    protected:
        W::Order<uint64_t> order;
        
    private:
        bool hasSorting;
        bool hasFilter;
        bool hasData;
        W::Vocabulary* sorting;
        W::Vocabulary* filter;
        
        static uint64_t counter;
    };
}

#endif // CATALOGUE_H
