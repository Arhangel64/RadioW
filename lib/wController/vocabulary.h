#ifndef CONTROLLER_VOCABULARY_H
#define CONTROLLER_VOCABULARY_H

#include "controller.h"

#include <wType/address.h>
#include <wType/object.h>
#include <wType/event.h>
#include <wType/vector.h>
#include <wType/vocabulary.h>

namespace C {
    class Vocabulary : public C::Controller
    {
    public:
        Vocabulary(const W::Address p_address, QObject* parent);
        ~Vocabulary();
        
    signals:
        void clear();
        void newElement(const W::String& key, const W::Object& element);
        void removeElement(const W::String& key);
        
    protected:
        W::Vocabulary data;
        
        handler(get)
        handler(change)
        handler(clear)
    private:
        static uint64_t counter;
    };
}

#endif // VOCABULARY_H
