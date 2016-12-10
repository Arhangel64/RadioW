#ifndef CONTROLLER_LIST_H
#define CONTROLLER_LIST_H

#include "controller.h"

#include <wType/address.h>
#include <wType/object.h>
#include <wType/event.h>
#include <wType/vector.h>

namespace C {
    class List : public C::Controller
    {
    public:
        List(const W::Address p_address, QObject* parent);
        ~List();
        
    signals:
        void clear();
        void newElement(const W::Object& element);
        
    protected:
        W::Vector data;
        
        handler(get)
        handler(push)
        handler(clear)
    private:
        static uint64_t counter;
    };
}



#endif // CONTROLLER_LIST_H
