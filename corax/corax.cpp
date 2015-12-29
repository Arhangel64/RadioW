#include "corax.h"

#include <iostream>

using std::cout;
using std::endl;

Corax::Corax(QObject *parent):
    QObject(parent),
    server(new W::Server(W::String(U"corax"), this)),
    socket(new W::Socket(W::String(U"corax_test"), this))
{
    connect(server, SIGNAL(newConnection(const W::Socket&)), SLOT(onNewConnection(const W::Socket&)));

    server->listen(8080);
    
    connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    socket->open(W::String(U"localhost"), W::Uint64(8080));
}

void Corax::onNewConnection(const W::Socket& socket)
{
    cout << "New connection on sever side" << endl;
}

void Corax::onSocketConnected()
{
    cout << "Client handshaked on client side" << endl;
}