#include "corax.h"

#include <iostream>

using std::cout;
using std::endl;

Corax::Corax(QObject *parent):
    QObject(parent),
    server(new W::Server(W::String(u"corax"), this))
{
    connect(server, SIGNAL(newConnection(const W::Socket&)), SLOT(onNewConnection(const W::Socket&)));

    server->listen(8080);
}

void Corax::onNewConnection(const W::Socket& socket)
{
    cout << "New connection on sever side" << endl;
}