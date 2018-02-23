#ifndef M_VOCABULARY_H
#define M_VOCABULARY_H

#include "model.h"

#include <wType/object.h>
#include <wType/address.h>
#include <wType/vocabulary.h>
#include <wType/vector.h>


namespace M {
    class ICatalogue;
    
    class Vocabulary : public M::Model 
    {
        friend class ICatalogue;
    public:
        Vocabulary(const W::Address p_address, QObject* parent = 0);
        Vocabulary(W::Vocabulary* p_data, const W::Address p_address, QObject* parent = 0);
        ~Vocabulary();
        
        void insert(const W::String& key, const W::Object& value);
        void insert(const W::String& key, W::Object* value);
        void erase(const W::String& key);
        void clear();
        
        void set(const W::Object & value) override;
        void set(W::Object* value) override;
        
        M::Model::ModelType getType() const override;
        static const M::Model::ModelType type = vocabulary;
        
    protected:
        void h_subscribe(const W::Event & ev) override;
        
        handler(get);
        
    private:
        W::Vocabulary* data;
        
    };
}

#endif // M_VOCABULARY_H
