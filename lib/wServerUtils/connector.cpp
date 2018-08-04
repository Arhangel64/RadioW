#include "connector.h"
#include "commands.h"

U::Connector::Connector(W::Dispatcher* dp, W::Server* srv, U::Commands* cmds, QObject* parent):
    QObject(parent),
    dispatcher(dp),
    server(srv),
    commands(cmds),
    nodes(),
    ignoredNodes(),
    controllers()
{
    connect(server, SIGNAL(newConnection(const W::Socket&)), SLOT(onNewConnection(const W::Socket&)));
    connect(server, SIGNAL(closedConnection(const W::Socket&)), SLOT(onClosedConnection(const W::Socket&)));
    
    W::String cn = W::String(u"connect");
    W::Handler* ch = W::Handler::create(commands->getAddress() + W::Address({cn}), this, &U::Connector::_h_connect);
    W::Vocabulary vc;
    vc.insert(u"address", W::Uint64(W::Object::string));
    vc.insert(u"port", W::Uint64(W::Object::uint64));
    commands->addCommand(cn, ch, vc);
    commands->enableCommand(cn, true);
}

U::Connector::~Connector()
{
    commands->removeCommand(W::String(u"connect"));
    Map::const_iterator itr = nodes.begin();
    Map::const_iterator end = nodes.begin();
    
    W::String dc = W::String(u"disconnect");
    for (; itr != end; ++itr) {
        commands->removeCommand(dc + itr->first);
    }
    
    std::multimap<W::String, C::Controller*>::const_iterator cbeg = controllers.begin();
    std::multimap<W::String, C::Controller*>::const_iterator cend = controllers.end();
    
    for (; cbeg != cend; ++cbeg) {
        C::Controller* ctrl = cbeg->second;
        if (ctrl->isSubscribed()) {
            ctrl->unsubscribe();
            ctrl->unregisterController();
        }
    }
}

void U::Connector::addIgnoredNode(const W::String& name)
{
    ignoredNodes.insert(name);
}

void U::Connector::sendTo(const W::String& name, const W::Event& event)
{
    Map::const_iterator itr = nodes.find(name);
    if (itr != nodes.end()) {
        throw new NodeAccessError(name);
    } else {
        server->getConnection(itr->second)->send(event);
    }
}

void U::Connector::onNewConnection(const W::Socket& socket)
{
    W::String name = socket.getRemoteName();
    std::set<W::String>::const_iterator ign = ignoredNodes.find(name);
    if (ign == ignoredNodes.end()) {
        Map::const_iterator itr = nodes.find(name);
        if (itr == nodes.end()) {
            if (server->getName() == name) {
                emit serviceMessage("An attempt to connect node to itself, closing connection");
                server->closeConnection(socket.getId());
            } else {
                W::String dc = W::String(u"disconnect");
                W::String dn = dc + name;
                W::Handler* dh = W::Handler::create(commands->getAddress() + W::Address({dc, name}), this, &U::Connector::_h_disconnect);
                commands->addCommand(dn, dh, W::Vocabulary());
                commands->enableCommand(dn, true);
                
                nodes.insert(std::make_pair(name, socket.getId()));
                
                emit serviceMessage(QString("New connection, id: ") + socket.getId().toString().c_str());
                connect(&socket, SIGNAL(message(const W::Event&)), dispatcher, SLOT(pass(const W::Event&)));
                
                std::multimap<W::String, C::Controller*>::const_iterator beg = controllers.lower_bound(name);
                std::multimap<W::String, C::Controller*>::const_iterator end = controllers.upper_bound(name);
                
                for (; beg != end; ++beg) {
                    beg->second->registerController(dispatcher, &socket);
                    beg->second->subscribe();
                }
                
                emit nodeConnected(name);
            }
        } else {
            emit serviceMessage(QString("Node ") + QString(name.toString().c_str()) + " tried to connect, but connection with that node is already open, closing new connection");
            server->closeConnection(socket.getId());
        }
    } else {
        emit serviceMessage(QString("New connection, id: ") + socket.getId().toString().c_str());
        connect(&socket, SIGNAL(message(const W::Event&)), dispatcher, SLOT(pass(const W::Event&)));
    }
}

void U::Connector::onClosedConnection(const W::Socket& socket)
{
    emit serviceMessage(QString("Connection closed, id: ") + socket.getId().toString().c_str());
    
    W::String name = socket.getRemoteName();
    std::set<W::String>::const_iterator ign = ignoredNodes.find(name);
    if (ign == ignoredNodes.end()) {
        Map::const_iterator itr = nodes.find(name);
        if (itr != nodes.end()) {
            std::multimap<W::String, C::Controller*>::const_iterator beg = controllers.lower_bound(name);
            std::multimap<W::String, C::Controller*>::const_iterator end = controllers.upper_bound(name);
            
            for (; beg != end; ++beg) {
                beg->second->unsubscribe();
                beg->second->unregisterController();
            }
            
            emit nodeDisconnected(name);
            commands->removeCommand(W::String(u"disconnect") + name);
            nodes.erase(itr);
        }
    }
}

void U::Connector::h_connect(const W::Event& ev)
{
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    const W::String& addr = static_cast<const W::String&>(vc.at(u"address"));
    const W::Uint64& port = static_cast<const W::Uint64&>(vc.at(u"port"));
    server->openConnection(addr, port);
}

void U::Connector::h_disconnect(const W::Event& ev)
{
    const W::Address& addr = static_cast<const W::Address&>(ev.getDestination());
    const W::String& name = addr.back();
    
    Map::const_iterator itr = nodes.find(name);
    server->closeConnection(itr->second);
}

const W::Socket* U::Connector::getNodeSocket(const W::String& name)
{
    Map::const_iterator itr = nodes.find(name);
    if (itr == nodes.end()) {
        throw new NodeAccessError(name);
    }
    return server->getConnection(itr->second);
}

void U::Connector::registerHandler(W::Handler* handler)
{
    dispatcher->registerHandler(handler);
}

void U::Connector::unregisterHandler(W::Handler* handler)
{
    dispatcher->unregisterHandler(handler);
}

const W::Socket * U::Connector::getConnection(uint64_t p_id) const
{
    return server->getConnection(p_id);
}

void U::Connector::passThroughDispatcher(const W::Event& ev) const
{
    dispatcher->pass(ev);
}

void U::Connector::registerController(C::Controller* ctrl, const W::String& node)
{
    std::set<W::String>::const_iterator iitr = ignoredNodes.find(node);
    if (iitr != ignoredNodes.end()) {
        throw 3;    //this means you're trying to receive something from one of ignored nodes, which never going to be handled in connector, most probably it's a mistake
    }
    controllers.insert(std::make_pair(node, ctrl));
    Map::const_iterator itr = nodes.find(node);
    if (itr != nodes.end()) {
        ctrl->registerController(dispatcher, server->getConnection(itr->second));
        ctrl->subscribe();              //let's say I always need them subscribed, for now at least
    }
}

void U::Connector::unregisterController(C::Controller* ctrl, const W::String& node)
{
    bool found = false;
    std::multimap<W::String, C::Controller*>::const_iterator beg = controllers.lower_bound(node);
    std::multimap<W::String, C::Controller*>::const_iterator end = controllers.upper_bound(node);
    
    for (; beg != end; ++beg) {
        if (beg->second == ctrl) {
            found = true;           //TODO make a proper way to store 'em
            break;
        }
    }
    
    if (!found) {
        throw 4;
    }
    
    if (ctrl->isSubscribed()) {
        ctrl->unsubscribe();
        ctrl->unregisterController();
    }
    controllers.erase(beg);
}

