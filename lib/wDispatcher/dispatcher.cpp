#include "dispatcher.h"

#include "handler.h"

W::Dispatcher::Dispatcher(QObject* parent): 
    QObject(parent)
{}

W::Dispatcher::~Dispatcher()
{}

void W::Dispatcher::pass(const W::Event& ev) const
{

    n_map::const_iterator itr;
    itr = nodes.find(ev.getDestination());
        
    if (itr != nodes.end())
    {
        itr->second->pass(ev);
    }
    else
    {
        //TODO default handler;
    }
}

void W::Dispatcher::registerHandler(W::Handler* dp)
{
    nodes.insert(std::make_pair(dp->getAddress(), dp));
}

void W::Dispatcher::unregisterHandler(W::Handler* dp)
{
    n_map::const_iterator itr = nodes.find(dp->getAddress());
    if (itr != nodes.end())
    {
        nodes.erase(itr);
    }
    else
    {
        //TODO exception;
    }
}