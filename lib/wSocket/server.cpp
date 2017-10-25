#include "server.h"

#include <iostream>

using std::cout;
using std::endl;

W::Server::Server(const W::String& name, QObject* parent): 
    QObject(parent),
    lastId(0),
    pool(),
    connections(),
    server(0),
    name(name)
{
    server = new QWebSocketServer(name.toString().c_str(), QWebSocketServer::NonSecureMode, this);
    connect(server, SIGNAL(newConnection()), SLOT(onNewConnection()));
    connect(server, SIGNAL(serverError(QWebSocketProtocol::CloseCode)), SLOT(onServerError(QWebSocketProtocol::CloseCode)));
}

W::Server::~Server()
{

}

void W::Server::listen(uint16_t port)
{
    if (server->listen(QHostAddress::Any, port)){
        
    }
    
}

void W::Server::stop()
{
    server->close();
    lastId = 0;
    pool.clear();
    std::map<uint64_t, Socket*>::const_iterator it;
    std::map<uint64_t, Socket*>::const_iterator end = connections.end();
    
    for (it = connections.begin(); it != end; ++it) {
        it->second->close();
    }
}

const W::Socket& W::Server::getConnection(uint64_t p_id) const
{
    std::map<uint64_t, Socket*>::const_iterator itr = connections.find(p_id);
    if (itr == connections.end()) {
        throw 2;
    }
    
    return *(itr->second);
}

uint64_t W::Server::getConnectionsCount() const
{
    return connections.size();
}

void W::Server::onNewConnection()
{
    QWebSocket *webSocket = server->nextPendingConnection();
    Socket* wSocket = createSocket(webSocket);
    wSocket->setRemoteId();
}

void W::Server::onSocketConnected() {
    Socket* socket = static_cast<Socket*>(sender());
    emit newConnection(*socket);
    emit connectionCountChange(getConnectionsCount());
}

void W::Server::onSocketDisconnected() {
    Socket* socket = static_cast<Socket*>(sender());
    uint64_t socketId = socket->getId();
    std::map<uint64_t, Socket*>::const_iterator it = connections.find(socketId);
    connections.erase(it);
    pool.insert(socketId);
    emit closedConnection(*socket);
    emit connectionCountChange(getConnectionsCount());
    socket->deleteLater();
}

void W::Server::onServerError(QWebSocketProtocol::CloseCode code)
{
    cout << "Server error: " << code << endl;
}

void W::Server::closeConnection(uint64_t p_id)
{
    std::map<uint64_t, Socket*>::const_iterator itr = connections.find(p_id);
    if (itr == connections.end()) {
        throw 2;
    }
    
    itr->second->close();
}

W::Socket * W::Server::createSocket(QWebSocket* socket)
{
    uint64_t connectionId;
    if (pool.empty()) {
        connectionId = ++lastId;
    } else {
        std::set<uint64_t>::const_iterator itr = pool.begin();
        connectionId = *itr;
        pool.erase(itr);
    }
    Socket *wSocket = new Socket(name, socket, connectionId, this);
    
    connections[connectionId] = wSocket;
    connect(wSocket, SIGNAL(connected()), SLOT(onSocketConnected()));
    connect(wSocket, SIGNAL(disconnected()), SLOT(onSocketDisconnected()));
    connect(wSocket, SIGNAL(negotiationId(uint64_t)), SLOT(onSocketNegotiationId(uint64_t)));
    
    return wSocket;
}


void W::Server::openConnection(const W::String& addr, const W::Uint64& port)
{
    QWebSocket *webSocket = new QWebSocket();
    Socket* wSocket = createSocket(webSocket);
    wSocket->open(addr, port);
    
}

void W::Server::onSocketNegotiationId(uint64_t p_id)
{
    Socket* socket = static_cast<Socket*>(sender());
    
    if (p_id == socket->id) {
        socket->setRemoteName();
    } else {
        std::set<uint64_t>::const_iterator pItr = pool.lower_bound(p_id);
        uint64_t newId;
        if (pItr == pool.end()) {
            newId = ++lastId;
        } else {
            newId = *pItr;
            pool.erase(pItr);
        }
        std::map<uint64_t, Socket*>::const_iterator itr = connections.find(socket->id);
        connections.erase(itr);
        pool.insert(socket->id);
        socket->id = Uint64(newId);
        connections[newId] = socket;
        socket->setRemoteId();
    }
}

W::String W::Server::getName() const
{
    return name;
}

