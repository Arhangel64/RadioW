#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <map>

#include <wType/string.h>
#include <wType/address.h>
#include <wType/event.h>

#include "handler.h"
#include "defaulthandler.h"

#include <wContainer/order.h>

namespace W 
{
    class Dispatcher
    {
    public:
        Dispatcher();
        ~Dispatcher();

        void pass(const W::Event& ev) const;
        
        void registerHandler(W::Handler* dp);
        void unregisterHandler(W::Handler* dp);
        
        void registerDefaultHandler(W::DefaultHandler* dh);
        void unregisterDefaultHandler(W::DefaultHandler* dh);
        
    protected:
        typedef std::map<W::Address, W::Order<W::Handler*>> n_map;
        typedef W::Order<W::DefaultHandler*> d_order;

        n_map nodes;
        d_order defaultHandlers;
        
    };
}

#endif // DISPATCHER_H
