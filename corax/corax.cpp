#include "corax.h"

#include <iostream>

using std::cout;
using std::endl;

Corax* Corax::corax = 0;

Corax::Corax(QObject *parent):
    QObject(parent),
    server(new W::Server(W::String(u"corax"), this)),
    logger(new W::Logger()),
    h_test(0),
    modelName(W::String(u"Corax"), W::Address({u"name"})),
    dispatcher(new W::Dispatcher())
{
    if (corax != 0) 
    {
        throw SingletonError();
    }
    Corax::corax = this;
    
    connect(server, SIGNAL(newConnection(const W::Socket&)), SLOT(onNewConnection(const W::Socket&)));
    connect(&modelName, SIGNAL(serviceMessage(const QString&)), SLOT(onModelServiceMessage(const QString&)));
    
    dispatcher->registerDefaultHandler(logger);
    
    h_test = W::Handler::create(W::Address({u"corax", u"test"}), this, &Corax::test);
    dispatcher->registerHandler(h_test);
}

Corax::~Corax()
{
    dispatcher->unregisterHandler(h_test);
    dispatcher->unregisterDefaultHandler(logger);
    
    delete logger;
    delete dispatcher;
    delete h_test;
    
    Corax::corax = 0;
}

void Corax::onNewConnection(const W::Socket& socket)
{
    cout << "New connection, id: " << socket.getId().toString() << endl;
    connect(&socket, SIGNAL(message(const W::Event&)), dispatcher, SLOT(pass(const W::Event&)));
    connect(&socket, SIGNAL(disconnected()), SLOT(onSocketDisconnected()));
}

void Corax::onSocketDisconnected()
{
    W::Socket* socket = static_cast<W::Socket*>(sender());
    cout << "Connection closed, id: " << socket->getId().toString() << endl;
}

void Corax::start()
{
    cout << "Starting corax..." << endl;
    server->listen(8080);
    cout << "Registering models..." << endl;
    modelName.registerModel(dispatcher, server);
    cout << "Corax is ready" << endl;
}

void Corax::stop()
{
    cout << "Stopping corax..." << endl;
    modelName.unregisterModel();
    server->stop();
}

void Corax::test(const W::Event& ev)
{
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(ev.getData());
    cout << "New test event, data: " << vc.toString() << endl;
    
    W::Vocabulary *nvc = new W::Vocabulary();
    nvc->insert(u"message", W::String(u"Hello, I'm Corax."));
    nvc->insert(u"source", h_test->getAddress());
    
    W::Event nev(static_cast<const W::Address&>(vc.at(u"source")), nvc);
    server->getConnection(ev.getSenderId()).send(nev);
}

void Corax::onModelServiceMessage(const QString& msg)
{
    cout << msg.toStdString() << endl;
}

