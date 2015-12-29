#include "server.h"

#include <iostream>

using std::cout;
using std::endl;

W::Server::Server(const W::String& name, QObject* parent): 
    QObject(parent),
    lastId(0),
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
    if (server->listen(QHostAddress::Any, port))
    {
        
    }
    
}

void W::Server::stop()
{
    server->close();
    lastId = 0;
    std::map<uint64_t, Socket*>::const_iterator it;
    std::map<uint64_t, Socket*>::const_iterator end = connections.end();
    
    for (it = connections.begin(); it != end; ++it) {
        it->second->close();
    }
}

void W::Server::onNewConnection()
{
    QWebSocket *webSocket = server->nextPendingConnection();
    ++lastId;
    Socket *wSocket = new Socket(name, webSocket, lastId, this);
    
    connections[lastId] = wSocket;
    connect(wSocket, SIGNAL(connected()), SLOT(onSocketConnected()));
    connect(wSocket, SIGNAL(disconnected()), SLOT(onSocketDisconnected()));
}

void W::Server::onSocketConnected() {
    Socket* socket = static_cast<Socket*>(sender());
    emit newConnection(*socket);
}

void W::Server::onSocketDisconnected() {
    Socket* socket = static_cast<Socket*>(sender());
    std::map<uint64_t, Socket*>::const_iterator it = connections.find(socket->getId());
    connections.erase(it);
    socket->deleteLater();
}

void W::Server::onServerError(QWebSocketProtocol::CloseCode code)
{
    cout << "Server error: " << code << endl;
}