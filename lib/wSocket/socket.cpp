#include "socket.h"

W::Socket::Socket(const W::String& p_name, QObject* parent): 
    QObject(parent),
    serverCreated(false),
    state(disconnected_s),
    socket(new QWebSocket()),
    id(0),
    name(p_name),
    remoteName()
{
    socket->setParent(this);
    setHandlers();
}

W::Socket::Socket(const W::String& p_name, QWebSocket* p_socket, QObject* parent): 
    QObject(parent),
    serverCreated(true),
    state(connecting_s),
    socket(p_socket),
    id(0),
    name(p_name),
    remoteName()
{
    socket->setParent(this);
    setHandlers();
}

W::Socket::~Socket()
{

}

void W::Socket::setHandlers() {
    connect(socket, SIGNAL(connected()), SLOT(onSocketConnected()));
    connect(socket, SIGNAL(disconnected()), SLOT(onSocketDisconnected()));
    connect(socket, SIGNAL(binaryMessageReceived(const QByteArray&)), SLOT(onBinaryMessageReceived(const QByteArray&)));
}

void W::Socket::onSocketConnected()
{

}

void W::Socket::onSocketDisconnected()
{
    id = W::Uint64(0);
    state = disconnected_s;
    emit disconnected();
}

void W::Socket::onBinaryMessageReceived(const QByteArray& ba)
{

}
