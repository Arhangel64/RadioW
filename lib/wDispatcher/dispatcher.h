#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <map>
#include <list>

#include <wType/string.h>
#include <wType/address.h>
#include <wType/event.h>

#include <QtCore/QObject>

#include "handler.h"
#include "defaulthandler.h"

#include <wContainer/order.h>

namespace W 
{
    class Dispatcher: 
        public QObject
    {
        Q_OBJECT
        
    public:
        Dispatcher();
        ~Dispatcher();
        
        void registerHandler(W::Handler* dp);
        void unregisterHandler(W::Handler* dp);
        
        void registerDefaultHandler(W::DefaultHandler* dh);
        void unregisterDefaultHandler(W::DefaultHandler* dh);
        
    public slots:
        void pass(const W::Event& ev) const;
        
    protected:
        typedef std::map<W::Address, W::Order<W::Handler*>> n_map;
        typedef W::Order<W::DefaultHandler*> d_order;

        n_map nodes;
        d_order defaultHandlers;
        
    };
}

#endif // DISPATCHER_H
