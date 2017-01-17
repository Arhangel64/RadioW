#ifndef M_STRING_H
#define M_STRING_H

#include "model.h"

#include <QtCore/QObject>

#include <wType/string.h>
#include <wType/address.h>

namespace M {
    
    class String: public Model
    {
    public:
        String(const W::String& str, const W::Address& addr, QObject* parent = 0);
        String(W::String* str, const W::Address& addr, QObject* parent = 0);
        
        ~String();
        
        void set(const W::Object & value) override;
        void set(W::Object * value) override;
        void set(const W::String& str);
        void set(W::String* str);
        
        M::Model::ModelType getType() const override;
        static const M::Model::ModelType type = M::Model::string;
        
    protected:
        void h_subscribe(const W::Event& ev);
        handler(get)
        
    private:
        W::String* data;
        
    };
}


#endif // M_STRING_H
