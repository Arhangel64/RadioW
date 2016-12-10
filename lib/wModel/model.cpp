#include "model.h"

M::Model::Model(const W::Address p_address, QObject* parent):
    QObject(parent),
    address(p_address),
    registered(false),
    dispatcher(0),
    server(0),
    subscribers(new Map()),
    subscribersCount(0),
    handlers(new HList()),
    properties(new W::Vector()),
    models(new MList())
{
    W::Handler* subscribe = W::Handler::create(address + W::Address({u"subscribe"}), this, &M::Model::_h_subscribe);
    W::Handler* unsubscribe = W::Handler::create(address + W::Address({u"unsubscribe"}), this, &M::Model::_h_unsubscribe);
    addHandler(subscribe);
    addHandler(unsubscribe);
}

M::Model::~Model()
{
    if (registered) {
        unregisterModel();
    }
    
    MList::iterator itr = models->begin();
    MList::iterator end = models->end();
        
    for (; itr != end; ++itr) {
        delete *itr;
    }
    
    HList::iterator hItr = handlers->begin();
    HList::iterator hEnd = handlers->end();
        
    for (; hItr != hEnd; ++hItr) {
        delete *hItr;
    }

    delete subscribers;
    delete properties;
    delete handlers;
    delete models;
}

void M::Model::addModel(M::Model* model)
{
    models->push_back(model);
    connect(model, SIGNAL(serviceMessage(const QString&)), SIGNAL(serviceMessage(const QString&)));
    if (registered) {
        model->registerModel(dispatcher, server);
    }
}

void M::Model::addHandler(W::Handler* handler)
{
    handlers->push_back(handler);
    if (registered) {
        dispatcher->registerHandler(handler);
    }
}

void M::Model::addProperty(const W::String& value, const W::String& name)
{
    W::Vocabulary vc;
    vc.insert(u"key", name);
    vc.insert(u"property", value);
    
    properties->push(vc);
    
    if (registered) {
        W::Vocabulary* nvc = new W::Vocabulary;
        nvc->insert(u"properties", *properties);
        broadcast(nvc, W::Address({u"properties"}));
    }
}


W::Address M::Model::getAddress() const
{
    return address;
}


void M::Model::registerModel(W::Dispatcher* dp, W::Server* srv)
{
    if (registered) {
        emit serviceMessage(QString("Model ") + address.toString().c_str() + " is already registered");
        throw 1;
    } else {
        dispatcher = dp;
        server = srv;
        
        MList::iterator itr = models->begin();
        MList::iterator end = models->end();
        
        for (; itr != end; ++itr) {
            M::Model* model = *itr;
            model->registerModel(dispatcher, server);
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

void M::Model::unregisterModel()
{
    if (!registered) {
        emit serviceMessage(QString("Model ") + address.toString().c_str() + " is not registered");
        throw 2;
    } else {
        MList::iterator itr = models->begin();
        MList::iterator end = models->end();
        
        for (; itr != end; ++itr) {
            Model* model = *itr;
            model->unregisterModel();
        }
        
        HList::iterator hItr = handlers->begin();
        HList::iterator hEnd = handlers->end();
        
        for (; hItr != hEnd; ++hItr) {
            W::Handler* handler = *hItr;
            dispatcher->unregisterHandler(handler);
        }
        
        Map::iterator sItr = subscribers->begin();
        Map::iterator sEnd = subscribers->end();
        
        for (; sItr != sEnd; ++sItr) {
            const W::Socket& socket = server->getConnection(sItr->first);
            disconnect(&socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
        }
        subscribers->clear();
        subscribersCount = 0;
        
        dispatcher = 0;
        server = 0;
        
        registered = false;
    }
    
}

void M::Model::h_subscribe(const W::Event& ev)
{
    uint64_t id = ev.getSenderId();
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    const W::Address& source = static_cast<const W::Address&>(vc.at(u"source"));
    
    Map::iterator sItr = subscribers->find(id);
    
    if (sItr == subscribers->end()) {
        std::pair<Map::iterator, bool> pair = subscribers->emplace(std::make_pair(id, W::Order<W::Address>()));
        if (!pair.second) {
            emit serviceMessage(QString("Model ") + address.toString().c_str() + ": something completely wrong happened");
            throw 3;
        }
        const W::Socket& socket = server->getConnection(id);
        connect(&socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
        sItr = pair.first;
    }
    W::Order<W::Address>::const_iterator oItr = sItr->second.find(source);
    if (oItr != sItr->second.end()) {
        emit serviceMessage(QString("Socket ") + id + 
                            " subscriber " + source.toString().c_str() + 
                            " is already subscribed to model " + source.toString().c_str());
        throw 4;
    }
    
    sItr->second.push_back(source);
    ++subscribersCount;
    
    W::Vocabulary* nvc = new W::Vocabulary();
    nvc->insert(u"properties", *properties);
    
    response(nvc, W::Address({u"properties"}), ev);
    emit serviceMessage(QString("Model ") + address.toString().c_str() + ": now has " + std::to_string(subscribersCount).c_str() + " subscribers");
}

void M::Model::onSocketDisconnected()
{
    W::Socket* socket = static_cast<W::Socket*>(sender());
    disconnect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    uint64_t id = socket->getId();
    Map::iterator itr = subscribers->find(id);
    
    if (itr == subscribers->end()) {
        emit serviceMessage(QString("Model ") + address.toString().c_str() + 
                            ": socket disconnected have been handled for not subscribed id");
        throw 5;
    }
    
    subscribersCount -= itr->second.size();
    subscribers->erase(itr);
    
    emit serviceMessage(QString("Model ") + address.toString().c_str() + ": now has " + std::to_string(subscribersCount).c_str() + " subscribers");
}

void M::Model::h_unsubscribe(const W::Event& ev)
{
    uint64_t id = ev.getSenderId();
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    const W::Address& source = static_cast<const W::Address&>(vc.at(u"source"));
    
    Map::iterator itr = subscribers->find(id);
    if (itr == subscribers->end()) {
        emit serviceMessage(QString("Socket ") + id + 
                            " has no subscribed addresses to model " + source.toString().c_str());
        throw 6;
    }
    
    W::Order<W::Address>& ord = itr->second;
    W::Order<W::Address>::const_iterator oItr = ord.find(source);
    if (oItr == ord.end()) {
        emit serviceMessage(QString("Socket ") + id + 
                            " subscriber " + source.toString().c_str() + 
                            " is not subscribed to model " + source.toString().c_str());
        throw 7;
    }

    ord.erase(source);
    if (ord.size() == 0) {
        const W::Socket& socket = server->getConnection(itr->first);
        disconnect(&socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
        subscribers->erase(itr);
    }
    --subscribersCount;
    
    emit serviceMessage(QString("Model ") + address.toString().c_str() + ": now has " + std::to_string(subscribersCount).c_str() + " subscribers");
}

void M::Model::response(W::Vocabulary* vc, const W::Address& handlerAddress, const W::Event& src)
{
    if (!registered) {
        emit serviceMessage(QString("An attempt to send event from model ") + address.toString().c_str() + " which was not registered");
        throw 8;
    }
    const W::Vocabulary& svc = static_cast<const W::Vocabulary&>(src.getData());
    const W::Address& source = static_cast<const W::Address&>(svc.at(u"source"));
    uint64_t id = src.getSenderId();
    vc->insert(u"source", address);
    
    W::Event ev(source + handlerAddress, vc);
    ev.setSenderId(id);
    server->getConnection(id).send(ev);
}


void M::Model::broadcast(W::Vocabulary* vc, const W::Address& handlerAddress)
{
    if (!registered) {
        emit serviceMessage(QString("An attempt to send event from model ") + address.toString().c_str() + " which was not registered");
        throw 8;
    }
    Map::const_iterator itr = subscribers->begin();
    Map::const_iterator end = subscribers->end();
    vc->insert(u"source", address);
    
    for (;itr != end; ++itr) {
        W::Order<W::Address>::const_iterator oItr = itr->second.begin();
        W::Order<W::Address>::const_iterator oEnd = itr->second.end();
        for (;oItr != oEnd; ++oItr) {
            W::Event ev(*oItr + handlerAddress, vc->copy());
            ev.setSenderId(itr->first);
            server->getConnection(itr->first).send(ev);
        }
    }
    delete vc;
}
