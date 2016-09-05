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
    QMap<uint64_t, Service*>::iterator beg = services.begin();
    QMap<uint64_t, Service*>::iterator end = services.end();
    
    for (; beg != end; ++beg) {
        delete *beg;
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
    QMap<uint64_t, Service*>::iterator beg = services.begin();
    QMap<uint64_t, Service*>::iterator end = services.end();
    
    for (; beg != end; ++beg) {
        (*beg)->disconnect();
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
    Service* srv = services[id];
    QString dbg = srv->name + ": " + msg;
    debug(dbg.toStdString());
}


void Roboute::addService(const QMap<QString, QString>& params)
{
    Service* srv = Service::create(params);
    services.insert(srv->id, srv);
    
    connect(srv, SIGNAL(serviceMessage(const QString&)), this, SLOT(serviceMessage(const QString&)));
    
    emit newService(*srv);
}

void Roboute::connectService(uint64_t id)
{
    Service* srv = services[id];
    srv->connect();
}

void Roboute::disconnectService(uint64_t id)
{
    Service* srv = services[id];
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
