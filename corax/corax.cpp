#include <iostream>
#include <QtNetwork/QHostAddress>

#include "corax.h"

using std::cout;
using std::endl;

Corax::Corax(QObject *parent):
	QObject(parent),
	server(new WServer(QString("corax"), this)),
	socket(new WSocket(QString("corax_test"), this))
{
	connect(server, SIGNAL(newConnection(WSocket*)), SLOT(onNewConnection(WSocket*)));
	
	server->listen(QHostAddress::Any, 8080);
    
    
    connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    connect(socket, SIGNAL(t_ev(const QString&)), this, SLOT(onSocketEvent(const QString&)));
    socket->open(QUrl("ws://localhost:8080"));
}

void Corax::onNewConnection(WSocket* wsocket)
{
    connect(wsocket, SIGNAL(t_ev(const QString&)), this, SLOT(onServerEvent(const QString&)));
	cout << "New connection on sever side" << endl;
}

void Corax::onSocketConnected()
{
    cout << "Client handshaked on client side" << endl;
}

void Corax::onServerEvent(const QString& msg)
{
    cout << "Server event: " << msg.toStdString() << endl;
}

void Corax::onSocketEvent(const QString& msg)
{
    cout << "Socket event: " << msg.toStdString() << endl;
}