#include "corax.h"

#include <iostream>

using std::cout;
using std::endl;

Corax* Corax::corax = 0;

Corax::Corax(QObject *parent):
    QObject(parent),
    server(new W::Server(W::String(u"corax"), this)),
    logger(new W::Logger()),
    modelName(new M::String(W::String(u"Corax"), W::Address({u"name"}))),
    connectionsCount(new M::String(W::String(u"0"), W::Address({u"connectionsAmount"}))),
    managementCommands(new M::List(W::Address{u"managementCommands"})),
    dispatcher(new W::Dispatcher())
{
    if (corax != 0) 
    {
        throw SingletonError();
    }
    Corax::corax = this;
    
    connect(server, SIGNAL(newConnection(const W::Socket&)), SLOT(onNewConnection(const W::Socket&)));
    connect(modelName, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(managementCommands, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(connectionsCount, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    
    dispatcher->registerDefaultHandler(logger);
}

Corax::~Corax()
{
    dispatcher->unregisterDefaultHandler(logger);
    
    delete managementCommands;
    delete connectionsCount;
    delete modelName;
    
    delete logger;
    delete dispatcher;
    
    Corax::corax = 0;
}

void Corax::onNewConnection(const W::Socket& socket)
{
    cout << "New connection, id: " << socket.getId().toString() << endl;
    connect(&socket, SIGNAL(message(const W::Event&)), dispatcher, SLOT(pass(const W::Event&)));
    connect(&socket, SIGNAL(disconnected()), SLOT(onSocketDisconnected()));
    connectionsCount->set(new W::String(std::to_string(server->getConnectionsCount())));
}

void Corax::onSocketDisconnected()
{
    W::Socket* socket = static_cast<W::Socket*>(sender());
    cout << "Connection closed, id: " << socket->getId().toString() << endl;
    connectionsCount->set(new W::String(std::to_string(server->getConnectionsCount())));
}

void Corax::start()
{
    cout << "Starting corax..." << endl;
    server->listen(8080);
    cout << "Registering models..." << endl;
    modelName->registerModel(dispatcher, server);
    connectionsCount->registerModel(dispatcher, server);
    managementCommands->registerModel(dispatcher, server);
    cout << "Corax is ready" << endl;
}

void Corax::stop()
{
    cout << "Stopping corax..." << endl;
    managementCommands->unregisterModel();
    connectionsCount->unregisterModel();
    modelName->unregisterModel();
    server->stop();
}

void Corax::onModelServiceMessage(const QString& msg)
{
    cout << msg.toStdString() << endl;
}

