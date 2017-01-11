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
    commands(new Commands(W::Address{u"management"})),
    connector(0),
    dispatcher(new W::Dispatcher())
{
    if (corax != 0) 
    {
        throw SingletonError();
    }
    Corax::corax = this;
    
    connector = new Connector(W::String(u"Corax"), dispatcher, server, commands);
    
    connect(modelName, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(commands, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(connectionsCount, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(connector, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(connector, SIGNAL(connectionCountChange(uint64_t)), SLOT(onConnectionCountChanged(uint64_t)));
    
    dispatcher->registerDefaultHandler(logger);
    
    connector->addNode(W::String(u"Magnus"));
}

Corax::~Corax()
{
    delete connector;
    
    dispatcher->unregisterDefaultHandler(logger);
    
    delete commands;
    delete connectionsCount;
    delete modelName;
    
    delete logger;
    delete dispatcher;
    
    Corax::corax = 0;
}

void Corax::onConnectionCountChanged(uint64_t count)
{
    connectionsCount->set(new W::String(std::to_string(count)));
}

void Corax::start()
{
    cout << "Starting corax..." << endl;
    server->listen(8080);
    cout << "Registering models..." << endl;
    modelName->registerModel(dispatcher, server);
    connectionsCount->registerModel(dispatcher, server);
    commands->registerModel(dispatcher, server);
    cout << "Corax is ready" << endl;
}

void Corax::stop()
{
    cout << "Stopping corax..." << endl;
    commands->unregisterModel();
    connectionsCount->unregisterModel();
    modelName->unregisterModel();
    server->stop();
}

void Corax::onModelServiceMessage(const QString& msg)
{
    cout << msg.toStdString() << endl;
}
