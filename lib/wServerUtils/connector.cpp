#include "connector.h"

U::Connector::Connector(W::Dispatcher* dp, W::Server* srv, U::Commands* cmds, QObject* parent):
    QObject(parent),
    dispatcher(dp),
    server(srv),
    commands(cmds),
    nodes(),
    ignoredNodes()
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
}

void U::Connector::addIgnoredNode(const W::String& name)
{
    ignoredNodes.insert(name);
}

void U::Connector::sendTo(const W::String& name, const W::Event& event)
{
    Map::const_iterator itr = nodes.find(name);
    if (itr != nodes.end()) {
        throw 2;
    } else {
        server->getConnection(itr->second).send(event);
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
