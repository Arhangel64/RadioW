#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <map>

#include <QtCore/QObject>

#include <wType/string.h>
#include <wType/address.h>
#include <wType/event.h>

#include <wDispatcher/handler.h>

namespace W 
{
    class Handler;
    
    class Dispatcher: public QObject
    {
        Q_OBJECT
        
    public:
        Dispatcher(QObject* parent = 0);
        ~Dispatcher();

        void pass(const W::Event& ev) const;
        
        void registerHandler(W::Handler* dp);
        void unregisterHandler(W::Handler* dp);
        
    protected:
        typedef std::map<W::Address, W::Handler*> n_map;

        n_map nodes;
    };
}

#endif // DISPATCHER_H
