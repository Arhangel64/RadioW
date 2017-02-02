#include "perturabo.h"

#include <iostream>

using std::cout;
using std::endl;

Perturabo* Perturabo::perturabo = 0;

Perturabo::Perturabo(QObject *parent):
    QObject(parent),
    server(new W::Server(W::String(u"Perturabo"), this)),
    logger(new W::Logger()),
    attributes(new M::Attributes(W::Address({u"attributes"}))),
    commands(new U::Commands(W::Address{u"management"})),
    connector(0),
    dispatcher(new W::Dispatcher())
{
    if (perturabo != 0) 
    {
        throw SingletonError();
    }
    Perturabo::perturabo = this;
    
    connector = new U::Connector(W::String(u"Perturabo"), dispatcher, server, commands);
    
    connect(attributes, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(commands, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(connector, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    connect(connector, SIGNAL(connectionCountChange(uint64_t)), SLOT(onConnectionCountChanged(uint64_t)));
    
    dispatcher->registerDefaultHandler(logger);
    
    connector->addNode(W::String(u"Magnus"));
    connector->addNode(W::String(u"Corax"));
    
    attributes->addAttribute(W::String(u"connectionsCount"), new M::String(W::String(u"0"), W::Address({u"attributes", u"connectionCount"})));
    attributes->addAttribute(W::String(u"name"), new M::String(W::String(u"Perturabo"), W::Address({u"attributes", u"name"})));
    attributes->addAttribute(W::String(u"version"), new M::String(W::String(u"0.0.1"), W::Address({u"attributes", u"version"})));
}

Perturabo::~Perturabo()
{
    delete connector;
    
    dispatcher->unregisterDefaultHandler(logger);
    
    delete commands;
    delete attributes;
    
    delete logger;
    delete dispatcher;
    
    Perturabo::perturabo = 0;
}

void Perturabo::onConnectionCountChanged(uint64_t count)
{
    attributes->setAttribute(W::String(u"connectionsCount"), new W::String(std::to_string(count)));
}

void Perturabo::start()
{
    cout << "Starting perturabo..." << endl;
    server->listen(8082);
    cout << "Registering models..." << endl;
    attributes->registerModel(dispatcher, server);
    commands->registerModel(dispatcher, server);
    cout << "Perturabo is ready" << endl;
}

void Perturabo::stop()
{
    cout << "Stopping perturabo..." << endl;
    commands->unregisterModel();
    attributes->unregisterModel();
    server->stop();
}

void Perturabo::onModelServiceMessage(const QString& msg)
{
    cout << msg.toStdString() << endl;
}
