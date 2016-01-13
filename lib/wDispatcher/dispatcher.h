#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <map>

#include <wType/string.h>
#include <wType/address.h>

#include "node.h"

namespace W 
{
    class Handler;
    
    class Dispatcher: public Node
    {
        Q_OBJECT
        
    public:
        Dispatcher(const W::Address& p_rel_addr, QObject* parent = 0);
        ~Dispatcher();
        
        void setRelativeAddress(const W::Address& addr);
        void setAbsoluteAddress(const W::Address& addr);
        void pass(const W::Event& ev) const;
        
        void registerDispatcher(W::Dispatcher* dp);
        void unregisterDispatcher(W::Dispatcher* dp);
        
        void registerHandler(W::Handler* dp);
        void unregisterHandler(W::Handler* dp);
        
    signals:
        void absoluteAddressChanged(const W::Address&);
        
    protected:
        typedef std::map<W::Address, W::Node*> n_map;

        n_map nodes;
        W::Address abs_address;
        
    private slots:
        void onParentAbsoluteAddressChanged(const W::Address& addr);
        
    };
}

#endif // DISPATCHER_H
