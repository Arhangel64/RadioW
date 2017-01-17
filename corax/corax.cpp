#include "corax.h"

#include <iostream>

using std::cout;
using std::endl;

Corax* Corax::corax = 0;

Corax::Corax(QObject *parent):
    QObject(parent),
    server(new W::Server(W::String(u"corax"), this)),
    logger(new W::Logger()),
    attributes(new M::Attributes(W::Address({u"attributes"}))),
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
    
    connect(attributes, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(commands, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(connector, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(connector, SIGNAL(connectionCountChange(uint64_t)), SLOT(onConnectionCountChanged(uint64_t)));
    
    dispatcher->registerDefaultHandler(logger);
    
    connector->addNode(W::String(u"Magnus"));
    
    attributes->addAttribute(W::String(u"connectionsCount"), new M::String(W::String(u"0"), W::Address({u"attributes", u"connectionCount"})));
    attributes->addAttribute(W::String(u"name"), new M::String(W::String(u"Corax"), W::Address({u"attributes", u"name"})));
}

Corax::~Corax()
{
    delete connector;
    
    dispatcher->unregisterDefaultHandler(logger);
    
    delete commands;
    delete attributes;
    
    delete logger;
    delete dispatcher;
    
    Corax::corax = 0;
}

void Corax::onConnectionCountChanged(uint64_t count)
{
    attributes->setAttribute(W::String(u"connectionsCount"), new W::String(std::to_string(count)));
}

void Corax::start()
{
    cout << "Starting corax..." << endl;
    server->listen(8080);
    cout << "Registering models..." << endl;
    attributes->registerModel(dispatcher, server);
    commands->registerModel(dispatcher, server);
    cout << "Corax is ready" << endl;
}

void Corax::stop()
{
    cout << "Stopping corax..." << endl;
    commands->unregisterModel();
    attributes->unregisterModel();
    server->stop();
}

void Corax::onModelServiceMessage(const QString& msg)
{
    cout << msg.toStdString() << endl;
}
