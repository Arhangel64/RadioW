#include "dispatcher.h"

W::Dispatcher::Dispatcher()
{}

W::Dispatcher::~Dispatcher()
{}

void W::Dispatcher::pass(const W::Event& ev) const
{

    n_map::const_iterator itr;
    itr = nodes.find(ev.getDestination());
        
    if (itr != nodes.end())
    {
        W::Order<W::Handler*>::const_iterator beg = itr->second.begin();
        W::Order<W::Handler*>::const_iterator end = itr->second.end();
        
        std::list<W::Handler*> list(beg, end);
        std::list<W::Handler*>::const_iterator itr = list.begin();
        std::list<W::Handler*>::const_iterator tEnd = list.end();
        for (; itr != tEnd; ++itr) {
            (*itr)->pass(ev);
        }
    }
    else
    {
        d_order::const_iterator itr = defaultHandlers.begin();
        d_order::const_iterator end = defaultHandlers.end();
        
        for (; itr != end; ++itr) 
        {
            if ((*itr)->call(ev)){
                break;
            }
        }
    }
}

void W::Dispatcher::registerHandler(W::Handler* dp)
{
    n_map::iterator itr = nodes.find(dp->getAddress());
    if (itr == nodes.end()) {
        W::Order<W::Handler*> ord;
        itr = nodes.insert(std::make_pair(dp->getAddress(), ord)).first;
    }
    itr->second.push_back(dp);
}

void W::Dispatcher::unregisterHandler(W::Handler* dp)
{
    n_map::iterator itr = nodes.find(dp->getAddress());
    if (itr != nodes.end())
    {
        W::Order<W::Handler*>::const_iterator o_itr = itr->second.find(dp);
        if (o_itr != itr->second.end()) {
            itr->second.erase(dp);
        
            if (itr->second.size() == 0) {
                nodes.erase(itr);
            }
        }
        else
        {
            throw 5;//TODO exception;
        }
        
    }
    else
    {
        throw 5;//TODO exception;
    }
}

void W::Dispatcher::registerDefaultHandler(W::DefaultHandler* dh)
{
    defaultHandlers.push_back(dh);
}

void W::Dispatcher::unregisterDefaultHandler(W::DefaultHandler* dh)
{
    defaultHandlers.erase(dh);
}
