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

const W::Socket& W::Server::getConnection(uint64_t p_id) const
{
    std::map<uint64_t, Socket*>::const_iterator itr = connections.find(p_id);
    if (itr == connections.end()) 
    {
        throw 2;
    }
    
    return *(itr->second);
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
    String r_name = socket->getRemoteName();
    
    p_map::iterator itr = peers.find(r_name);
    if (itr == peers.end())
    {
        std::pair<p_map::iterator, bool> elem = peers.emplace(r_name, std::map<uint64_t, Socket*>());
        if (!elem.second)
        {
            throw 1; //TODO
        }
        itr = elem.first;
    }
    itr->second.insert(std::make_pair(socket->getId(), socket));
    emit newConnection(*socket);
}

void W::Server::onSocketDisconnected() {
    Socket* socket = static_cast<Socket*>(sender());
    std::map<uint64_t, Socket*>::const_iterator it = connections.find(socket->getId());
    p_map::const_iterator itn = peers.find(socket->getRemoteName());
    if (it != connections.end()) {
        connections.erase(it);
    }
    if (itn != peers.end())
    {
        peers.erase(itn);
    }
    socket->deleteLater();
}

void W::Server::onServerError(QWebSocketProtocol::CloseCode code)
{
    cout << "Server error: " << code << endl;
}