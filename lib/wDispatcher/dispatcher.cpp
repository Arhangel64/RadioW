#include "dispatcher.h"

#include "handler.h"

W::Dispatcher::Dispatcher(const W::Address& p_rel_addr, QObject* parent): 
    Node(p_rel_addr, parent),
    abs_address(p_rel_addr)
{

}

W::Dispatcher::~Dispatcher()
{

}

void W::Dispatcher::pass(const W::Event& ev) const
{
    W::Address postfix = ev.getDestination() << abs_address.size();
    
    n_map::const_iterator itr = nodes.end();
    n_map::const_iterator end = nodes.end();
    
    while (postfix.size() > 0 && itr == nodes.end())
    {
        itr = nodes.find(postfix);
        postfix = postfix >> 1;
    }
    
    if (itr != end)
    {
        itr->second->pass(ev);
    }
    else
    {
        //TODO default handler;
    }
}

void W::Dispatcher::registerDispatcher(W::Dispatcher* dp)
{
    dp->setAbsoluteAddress(abs_address);
    connect(this, SIGNAL(absoluteAddressChanged(const W::Address&)), dp, SLOT(onParentAbsoluteAddressChanged(const W::Address&)));
    
    nodes.insert(std::make_pair(dp->rel_address, dp));
}

void W::Dispatcher::unregisterDispatcher(W::Dispatcher* dp)
{
    n_map::const_iterator itr = nodes.find(dp->rel_address);
    if (itr != nodes.end())
    {
        W::Address addr;
        dp->setAbsoluteAddress(addr);
        disconnect(this, SIGNAL(absoluteAddressChanged(const W::Address&)), dp, SLOT(onParentAbsoluteAddressChanged(const W::Address&)));
    
        nodes.erase(itr);
    }
    else
    {
        //TODO exception;
    }
}

void W::Dispatcher::registerHandler(W::Handler* dp)
{
    nodes.insert(std::make_pair(dp->getRelativeAddress(), dp));
}

void W::Dispatcher::unregisterHandler(W::Handler* dp)
{
    n_map::const_iterator itr = nodes.find(dp->getRelativeAddress());
    if (itr != nodes.end())
    {
        nodes.erase(itr);
    }
    else
    {
        //TODO exception;
    }
}

void W::Dispatcher::setRelativeAddress(const W::Address& addr)
{
    W::Address absolute_prefix = abs_address >> rel_address.size();
    
    W::Node::setRelativeAddress(addr);
    
    setAbsoluteAddress(absolute_prefix);
}

void W::Dispatcher::setAbsoluteAddress(const W::Address& addr)
{
    abs_address = rel_address + addr;
    emit absoluteAddressChanged(addr);
}

void W::Dispatcher::onParentAbsoluteAddressChanged(const W::Address& addr)
{
    setAbsoluteAddress(addr);
}
