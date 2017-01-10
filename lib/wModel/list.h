#ifndef M_LIST_H
#define M_LIST_H

#include "model.h"

#include <wType/object.h>
#include <wType/address.h>
#include <wType/vector.h>

namespace M {
    class List : public M::Model
    {
    public:
        List(const W::Address p_address, QObject* parent = 0);
        ~List();
        
        void push(const W::Object& obj);
        void push(W::Object* obj);
        void clear();
        
    protected:
        void h_subscribe(const W::Event & ev) override;
        
        handler(get);
        
    private:
        W::Vector* data;
        
    };
}

#endif // M_LIST_H
