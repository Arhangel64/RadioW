#ifndef M_VOCABULARY_H
#define M_VOCABULARY_H

#include "model.h"

#include <wType/object.h>
#include <wType/address.h>
#include <wType/vocabulary.h>
#include <wType/vector.h>

namespace M {
    class Vocabulary : public M::Model 
    {
    public:
        Vocabulary(const W::Address p_address, QObject* parent = 0);
        ~Vocabulary();
        
        void insert(const W::String& key, const W::Object& value);
        void insert(const W::String& key, W::Object* value);
        void erase(const W::String& key);
        void clear();
        
    protected:
        void h_subscribe(const W::Event & ev) override;
        
        handler(get);
        
    private:
        W::Vocabulary* data;
        
    };
}

#endif // M_VOCABULARY_H
