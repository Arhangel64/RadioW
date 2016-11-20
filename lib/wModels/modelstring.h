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
        
    protected:
        void h_subscribe(const W::Event& ev);
        handler(get)
        
    private:
        W::String* data;
        
    };
}


#endif // M_STRING_H
