#include "roboute.h"

#include <iostream>

using std::cout;
using std::endl;

Roboute* Roboute::roboute = 0;

Roboute::Roboute(QObject *parent):
    QObject(parent),
    logger(new W::Logger()),
    services(),
    dispatcher(new W::Dispatcher())
{
    if (roboute != 0) 
    {
        throw SingletonError();
    }
    Roboute::roboute = this;
    
    dispatcher->registerDefaultHandler(logger);
}

Roboute::~Roboute()
{
    QMap<uint64_t, QPair<Service*, QThread*>>::iterator beg = services.begin();
    QMap<uint64_t, QPair<Service*, QThread*>>::iterator end = services.end();
    
    for (; beg != end; ++beg) {
        delete beg->first;
    }
    
    dispatcher->unregisterDefaultHandler(logger);
    
    delete logger;
    delete dispatcher;
    
    Roboute::roboute = 0;
}

void Roboute::start()
{
    debug("Starting roboute...");
    debug("Roboute is ready");
}

void Roboute::stop()
{
    debug("Stopping roboute...");
    QMap<uint64_t, QPair<Service*, QThread*>>::iterator beg = services.begin();
    QMap<uint64_t, QPair<Service*, QThread*>>::iterator end = services.end();
    
    for (; beg != end; ++beg) {
        beg->second->quit();
    }
}

void Roboute::debug(std::string str)
{
    cout << str << endl;
    QString dbg = str.c_str();
    dbg.append("\n");
    emit debugMessage(dbg);
}

void Roboute::debug(uint64_t id, const QString& msg)
{
    Service* srv = services[id].first;
    QString dbg = srv->name + ": " + msg;
    debug(dbg.toStdString());
}


void Roboute::addService(const QMap<QString, QString>& params)
{
    Service* srv = Service::create(params);
    QThread* thread = new QThread(this);
    srv->moveToThread(thread);
    QPair<Service*, QThread*> pair(srv, thread);
    services.insert(srv->id, pair);
    
    connect(srv, SIGNAL(serviceMessage(const QString&)), this, SLOT(serviceMessage(const QString&)));
    thread->start();
    
    emit newService(*srv);
}

void Roboute::connectService(uint64_t id)
{
    Service* srv = services[id].first;
    srv->connect();
}

void Roboute::disconnectService(uint64_t id)
{
    Service* srv = services[id].first;
    srv->disconnect();
}

void Roboute::launchService(uint64_t id)
{
}

void Roboute::stopService(uint64_t id)
{
}

void Roboute::serviceMessage(const QString& msg)
{
    Service* srv = static_cast<Service*>(sender());
    debug(srv->id, msg);
}
