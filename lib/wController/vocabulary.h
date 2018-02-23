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
        Q_OBJECT
    protected:
        Vocabulary(const W::Address p_address, const W::Address& my_address, QObject* parent = 0);  //for inheritors
    public:
        Vocabulary(const W::Address p_address, QObject* parent = 0);
        ~Vocabulary();
        
        const W::Object& at(const W::String& key) const;
        const W::Object& at(const W::String::u16string& key) const;
        bool has(const W::String& key) const;
        bool has(const W::String::u16string& key) const;
        
    signals:
        void clear();
        void newElement(const W::String& key, const W::Object& element);
        void removeElement(const W::String& key);
        void data();
        
    protected:
        W::Vocabulary* p_data;
        
        handler(get)
        handler(change)
        handler(clear)
        
        virtual void _newElement(const W::String& key, const W::Object& element);
        virtual void _removeElement(const W::String& key);
        virtual void _clear();
        
    private:
        static uint64_t counter;
    };
}

#endif // VOCABULARY_H
