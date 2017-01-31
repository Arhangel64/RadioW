#include "connector.h"

Connector::Connector(const W::String& pnk, W::Dispatcher* dp, W::Server* srv, Commands* cmds, QObject* parent):
    QObject(parent),
    parentNodeKey(pnk),
    dispatcher(dp),
    server(srv),
    commands(cmds),
    nodes(),
    pending(),
    occ(0)
{
    connect(server, SIGNAL(newConnection(const W::Socket&)), SLOT(onNewConnection(const W::Socket&)));
}

Connector::~Connector()
{
    Map::const_iterator beg = nodes.begin();
    Map::const_iterator end = nodes.end();
    
    for (; beg != end; ++beg) {
        if (beg->second->outgoing && (beg->second->pending || beg->second->connected)) {
            W::Socket* socket = beg->second->socket;
            disconnect(socket, SIGNAL(connected()), this, SLOT(outgoingSocketConnected()));
            disconnect(socket, SIGNAL(error(W::Socket::SocketError, const QString&)), 
                       this, SLOT(outgoingSocketError(W::Socket::SocketError, const QString&)));
            socket->deleteLater();
        }
        delete beg->second;
    }
}

void Connector::addNode(const W::String& name)
{
    Map::const_iterator itr = nodes.find(name);
    if (itr != nodes.end()) {
        throw 1;
    }
    P::Node* node = new P::Node(name);
    W::String cn = W::String(u"connect") + name;
    W::String dn = W::String(u"disconnect") + name;
    W::Handler* connect = W::Handler::create(commands->getAddress() + W::Address({cn}), node, &P::Node::_h_connect);
    W::Handler* disconnect = W::Handler::create(commands->getAddress() + W::Address({dn}), node, &P::Node::_h_disconnect);
    QObject::connect(node, SIGNAL(connect(const W::String&, const W::String&, const W::Uint64&)),
                     SLOT(connectTo(const W::String&, const W::String&, const W::Uint64&)));
    QObject::connect(node, SIGNAL(disconnect(const W::String&)),
                     SLOT(disconnectNode(const W::String&)));
    nodes.insert(std::make_pair(node->name, node));
    W::Vocabulary vc;
    vc.insert(u"address", W::Uint64(W::Object::string));
    vc.insert(u"port", W::Uint64(W::Object::uint64));
    commands->addCommand(cn, connect, vc);
    commands->addCommand(dn, disconnect, W::Vocabulary());
    commands->enableCommand(cn, true);
}

void Connector::sendTo(const W::String& name, const W::Event& event)
{
    Map::const_iterator itr = nodes.find(name);
    if (itr != nodes.end()) {
        throw 2;
    } else {
        if (!itr->second->connected) {
            if (itr->second->outgoing) {
                itr->second->socket->send(event);
            } else {
                server->getConnection(itr->second->socketId).send(event);
            }
        } else {
            //TODO;
        }
    }
}

void Connector::onNewConnection(const W::Socket& socket)
{
    
    emit serviceMessage(QString("New connection, id: ") + socket.getId().toString().c_str());
    connect(&socket, SIGNAL(message(const W::Event&)), dispatcher, SLOT(pass(const W::Event&)));
    connect(&socket, SIGNAL(disconnected()), SLOT(onSocketDisconnected()));
    emit connectionCountChange(getConnectionCount());
    
    Map::const_iterator itr = nodes.find(socket.getRemoteName());
    if (itr != nodes.end()) {
        if (!itr->second->connected) {
            itr->second->connected = true;
            commands->enableCommand(W::String(u"disconnect") + itr->second->name, true);
            commands->enableCommand(W::String(u"connect") + itr->second->name, false);
            if (!itr->second->outgoing) {
                itr->second->socketId = socket.getId();
            }
        } else {
            //TODO;
        }
    }
}

void Connector::onSocketDisconnected()
{
    W::Socket* socket = static_cast<W::Socket*>(sender());
    
    emit serviceMessage(QString("Connection closed, id: ") + socket->getId().toString().c_str());
    
    Map::const_iterator itr = nodes.find(socket->getRemoteName());
    if (itr != nodes.end()) {
        if (itr->second->connected) {
            if (itr->second->outgoing) {
                socket->deleteLater();
                --occ;
            }
            itr->second->connected = false;
            itr->second->pending = false;
            itr->second->outgoing = false;
            itr->second->socket = 0;
            itr->second->socketId = 0;
            
            commands->enableCommand(W::String(u"connect") + itr->second->name, true);
            commands->enableCommand(W::String(u"disconnect") + itr->second->name, false);
        } else {
            throw 3;
        }
    } else {
        
    }
    emit connectionCountChange(getConnectionCount());
}

void Connector::connectTo(const W::String& name, const W::String& address, const W::Uint64& port)
{
    Map::const_iterator itr = nodes.find(name);
    if (itr == nodes.end()) {
        throw 4;
    }
    if (!itr->second->connected) {
        W::Socket* socket = new W::Socket(parentNodeKey);
        connect(socket, SIGNAL(connected()), SLOT(outgoingSocketConnected()));
        connect(socket, SIGNAL(error(W::Socket::SocketError, const QString&)),
                SLOT(outgoingSocketError(W::Socket::SocketError, const QString&)));
        itr->second->outgoing = true;
        itr->second->connected = false;
        itr->second->pending = true;
        itr->second->socket = socket;
        pending.insert(std::make_pair(socket, itr->second));
        commands->enableCommand(W::String(u"connect") + name, false);
        commands->enableCommand(W::String(u"disconnect") + name, false);
        socket->open(address, port);
    } else {
        //TODO;
    }
}

void Connector::disconnectNode(const W::String& name)
{
    Map::const_iterator itr = nodes.find(name);
    if (itr == nodes.end()) {
        throw 4;
    }
    
    if (itr->second->connected) {
        commands->enableCommand(W::String(u"connect") + name, false);
        commands->enableCommand(W::String(u"disconnect") + name, false);
        
        if (itr->second->outgoing) {
            itr->second->socket->close();
        } else {
            server->closeConnection(itr->second->socketId);
        }
    } else {
        //TODO;
    }
}


void Connector::outgoingSocketConnected()
{
    W::Socket* socket = static_cast<W::Socket*>(sender());
    Voc::const_iterator itr = pending.find(socket);
    if (itr != pending.end()) {
        itr->second->pending = false;
        
        if (itr->second->name == socket->getRemoteName()) {
            ++occ;
            disconnect(socket, SIGNAL(error(W::Socket::SocketError, const QString&)),
                    this, SLOT(outgoingSocketError(W::Socket::SocketError, const QString&)));
            onNewConnection(*socket);
        } else {
            emit serviceMessage(QString("Error: outgoing socket to ") + itr->second->name.toString().c_str() +
                                " unexpectedly connected to " + socket->getRemoteName().toString().c_str());
            socket->deleteLater();
            itr->second->connected = false;
            itr->second->pending = false;
            itr->second->outgoing = false;
            itr->second->socket = 0;
            itr->second->socketId = 0;
            
            commands->enableCommand(W::String(u"connect") + itr->second->name, true);
            commands->enableCommand(W::String(u"disconnect") + itr->second->name, false);
            
        }
        
        pending.erase(itr);
    } else {
        //TODO;
    }
}

void Connector::outgoingSocketError(W::Socket::SocketError err, const QString& msg)
{
    W::Socket* socket = static_cast<W::Socket*>(sender());
    Voc::const_iterator itr = pending.find(socket);
    if (itr != pending.end()) {
        itr->second->pending = false;
        
        itr->second->socket = 0;
        commands->enableCommand(W::String(u"connect") + itr->second->name, true);
        commands->enableCommand(W::String(u"disconnect") + itr->second->name, false);
        
        pending.erase(itr);
    } else {
         //TODO
    }
    
    socket->deleteLater();
}

uint64_t Connector::getConnectionCount() const
{
    return server->getConnectionsCount() + occ;
}

P::Node::Node(const W::String& p_name):
    QObject(),
    socket(0),
    socketId(0),
    name(p_name),
    connected(false),
    outgoing(false),
    pending(false)
{
}


void P::Node::h_connect(const W::Event& ev)
{
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    const W::String& addr = static_cast<const W::String&>(vc.at(u"address"));
    const W::Uint64& port = static_cast<const W::Uint64&>(vc.at(u"port"));
    emit connect(name, addr, port);
}

void P::Node::h_disconnect(const W::Event& ev)
{
    emit disconnect(name);
}





