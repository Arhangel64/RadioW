#include "wserver.h"

WServer::WServer(const QString& name, QObject *parent):
    QObject(parent),
    lastId(0),
    connections(),
    server (new QWebSocketServer(name, QWebSocketServer::NonSecureMode, this)),
    name(name)
{
    connect(server, &QWebSocketServer::newConnection, this, &WServer::onNewConnection);
}

void WServer::listen(const QHostAddress &address, quint16 port)
{
    server->listen(address, port);
}

void WServer::close() 
{
    server->close();
    lastId = 0;
    QHash<quint64, WSocket*>::const_iterator it;
    QHash<quint64, WSocket*>::const_iterator end = connections.end();
    
    for (it = connections.begin(); it != end; ++it) {
	(*it)->close();
    }
}

void WServer::onNewConnection()
{
    QWebSocket *webSocket = server->nextPendingConnection();
    WSocket *wSocket = new WSocket(name, webSocket, this);
    while (connections.contains(++lastId))
    {

    }
    connections[lastId] = wSocket;
    connect(wSocket, SIGNAL(connected()), SLOT(onSocketConnected()));
    connect(wSocket, SIGNAL(disconnected()), SLOT(onSocketDisconnected()));
    wSocket->setId(lastId);
}

void WServer::onSocketConnected() {
    WSocket * socket = static_cast<WSocket*>(sender());
    emit newConnection(socket);
}

void WServer::onSocketDisconnected() {
    WSocket * socket = static_cast<WSocket*>(sender());
    connections.remove(socket->getId());
    delete socket;
}