#include "controller.h"

C::Controller::Controller(const W::Address p_address, QObject* parent):
    QObject(parent),
    pairAddress(p_address),
    address(),
    dispatcher(0),
    socket(0),
    registered(false),
    subscribed(false),
    controllers(new CList()),
    handlers(new HList()),
    properties(new W::Vector())
{
    W::Handler* props = W::Handler::create(address + W::Address({u"properties"}), this, &C::Controller::h_properties);
    addHandler(props);
}

C::Controller::~Controller()
{
    if (subscribed) {
        unsubscribe();
    }
    if (registered) {
        unregisterController();
    }
    
    CList::iterator itr = controllers->begin();
    CList::iterator end = controllers->end();
    
    for (; itr != end; ++itr) {
        delete *itr;
    }
    
    delete controllers;
    delete handlers;
    delete properties;
}

void C::Controller::addController(C::Controller* ctrl)
{
    controllers->push_back(ctrl);
    connect(ctrl, SIGNAL(serviceMessage(const QString&)), SIGNAL(serviceMessage(const QString&)));
    if (registered) {
        ctrl->registerController(dispatcher, socket);
    }
}

void C::Controller::addHandler(W::Handler* handler)
{
    handlers->push_back(handler);
    if (registered) {
        dispatcher->registerHandler(handler);
    }
}

void C::Controller::h_properties(const W::Event& event)
{
    delete properties;
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(event.getData());
    properties = static_cast<W::Vector*>(vc.at(u"properties").copy());
    
    emit serviceMessage("successfully received properties");
}

void C::Controller::registerController(W::Dispatcher* dp, W::Socket* sock)
{
     if (registered) {
        emit serviceMessage(QString("Controller ") + address.toString().c_str() + " is already registered");
        throw 1;
    } else {
        dispatcher = dp;
        socket = sock;
        
        CList::iterator itr = controllers->begin();
        CList::iterator end = controllers->end();
        
        for (; itr != end; ++itr) {
            C::Controller* ctrl = *itr;
            ctrl->registerController(dispatcher, socket);
        }
        
        HList::iterator hItr = handlers->begin();
        HList::iterator hEnd = handlers->end();
        
        for (; hItr != hEnd; ++hItr) {
            W::Handler* handler = *hItr;
            dispatcher->registerHandler(handler);
        }
        
        registered = true;
    }
}

void C::Controller::unregisterController()
{
    if (!registered) {
        emit serviceMessage(QString("Controller ") + address.toString().c_str() + " is already unregistered");
        throw 2;
    } else {
        CList::iterator itr = controllers->begin();
        CList::iterator end = controllers->end();
        
        for (; itr != end; ++itr) {
            Controller* ctrl = *itr;
            ctrl->unregisterController();
        }
        
        HList::iterator hItr = handlers->begin();
        HList::iterator hEnd = handlers->end();
        
        for (; hItr != hEnd; ++hItr) {
            W::Handler* handler = *hItr;
            dispatcher->unregisterHandler(handler);
        }
        
        dispatcher = 0;
        socket = 0;
        
        registered = false;
    }
}

void C::Controller::send(W::Vocabulary* vc, const W::Address& handlerAddress)
{
    if (!registered) {
        emit serviceMessage(QString("An attempt to send event from model ") + address.toString().c_str() + " which was not registered");
        throw 3;
    }
    vc->insert(u"source", address);
    W::Event ev(pairAddress + handlerAddress, vc);
    ev.setSenderId(socket->getId());
    socket->send(ev);
}

void C::Controller::subscribe()
{
    if (subscribed) {
        emit serviceMessage(QString("An attempt to subscribe model ") + address.toString().c_str() + " which is already subscribed");
        throw 3;
    }
    send(new W::Vocabulary(), W::Address{u"subscribe"});
    
    CList::iterator itr = controllers->begin();
    CList::iterator end = controllers->end();
    
    for (; itr != end; ++itr) {
        (*itr)->subscribe();
    }
    
    subscribed = true;
}

void C::Controller::unsubscribe()
{
    if (!subscribed) {
        emit serviceMessage(QString("An attempt to unsubscribe model ") + address.toString().c_str() + " which not subscribed");
        throw 3;
    }
    send(new W::Vocabulary(), W::Address{u"unsubscribe"});
    
    CList::iterator itr = controllers->begin();
    CList::iterator end = controllers->end();
    
    for (; itr != end; ++itr) {
        (*itr)->unsubscribe();
    }
    
    subscribed = false;
}




