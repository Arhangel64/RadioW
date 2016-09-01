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
}

void Roboute::debug(std::string str)
{
    cout << str << endl;
    QString dbg = str.c_str();
    dbg.append("\n");
    emit debugMessage(dbg);
}

void Roboute::addService(const QMap<QString, QString>& params)
{
    Service* srv = Service::create(params);
    services.insert(srv->id, srv);
    
    emit newService(*srv);
}

void Roboute::connectService(uint64_t id)
{
    
}

void Roboute::disconnectService(uint64_t id)
{
}

void Roboute::launchService(uint64_t id)
{
}

void Roboute::stopService(uint64_t id)
{
}
