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
    readSettings();
    debug("Roboute is ready");
}

void Roboute::stop()
{
    debug("Stopping roboute...");
    saveSettings();
    QMap<uint64_t, Service*>::iterator beg = services.begin();
    QMap<uint64_t, Service*>::iterator end = services.end();
    
    for (; beg != end; ++beg) {
        Service* srv = *beg;
        srv->disconnect();
        srv->unregisterControllers(dispatcher);
    }
}

void Roboute::debug(std::string str) const
{
    cout << str << endl;
    QString dbg = str.c_str();
    dbg.append("\n");
    emit debugMessage(dbg);
}

void Roboute::debug(uint64_t id, const QString& msg) const
{
    Service* srv = services[id];
    QString dbg = srv->name + ": " + msg;
    debug(dbg.toStdString());
}


void Roboute::addService(const QMap<QString, QString>& params)
{
    Service* srv = Service::create(params);
    addService(srv);
}

void Roboute::removeService(uint64_t id)
{
    QMap<uint64_t, Service*>::iterator itr = services.find(id);
    if (itr != services.end()) {
        Service* srv = *itr;
        debug(id, "removing...");
        srv->unregisterControllers(dispatcher);
        srv->disconnect();
        srv->deleteLater();
        services.erase(itr);
        emit serviceRemoved(id);
    }
}

void Roboute::addService(Service* srv)
{
    services.insert(srv->id, srv);
    
    connect(srv, SIGNAL(serviceMessage(const QString&)), this, SLOT(onServiceMessage(const QString&)));
    connect(srv, SIGNAL(connecting()), this, SLOT(onServiceConnecting()));
    connect(srv, SIGNAL(connected()), this, SLOT(onServiceConnected()));
    connect(srv, SIGNAL(disconnecting()), this, SLOT(onServiceDisconnecting()));
    connect(srv, SIGNAL(disconnected()), this, SLOT(onServiceDisconnected()));
    connect(srv, SIGNAL(launching()), this, SLOT(onServiceLaunching()));
    connect(srv, SIGNAL(launched()), this, SLOT(onServiceLaunched()));
    connect(srv, SIGNAL(stopping()), this, SLOT(onServiceStopping()));
    connect(srv, SIGNAL(stopped()), this, SLOT(onServiceStopped()));
    connect(srv, SIGNAL(attributeChanged(const QString&, const QString&)), this, SLOT(onAttributeChanged(const QString&, const QString&)));
    connect(srv, SIGNAL(log(const QString&)), this, SLOT(onServiceLog(const QString&)));
    
    srv->registerContollers(dispatcher);
    
    emit newService(srv->id, srv->name);
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
    Service* srv = services[id];
    srv->launch();
}

void Roboute::stopService(uint64_t id)
{
    Service* srv = services[id];
    srv->stop();
}

void Roboute::onServiceMessage(const QString& msg)
{
    Service* srv = static_cast<Service*>(sender());
    debug(srv->id, msg);
}

void Roboute::onServiceConnecting()
{
    Service* srv = static_cast<Service*>(sender());
    emit serviceConnecting(srv->id);
}

void Roboute::onServiceConnected()
{
    Service* srv = static_cast<Service*>(sender());
    emit serviceConnected(srv->id);
}

void Roboute::onServiceDisconnecting()
{
    Service* srv = static_cast<Service*>(sender());
    emit serviceDisconnecting(srv->id);
}

void Roboute::onServiceDisconnected()
{
    Service* srv = static_cast<Service*>(sender());
    emit serviceDisconnected(srv->id);
}

void Roboute::onServiceLog(const QString& msg)
{
    Service* srv = static_cast<Service*>(sender());
    emit log(srv->id, msg);
}

void Roboute::saveSettings() const
{
    debug("Saving settings...");
    
    QSettings settings;
    settings.beginGroup("services");
    
    QList<QVariant> list;
    
    QMap<uint64_t, Service*>::const_iterator beg = services.begin();
    QMap<uint64_t, Service*>::const_iterator end = services.end();
    
    for (; beg != end; ++beg) {
        list.push_back((*beg)->saveState());
    }
    settings.setValue("list", list);
    
    settings.endGroup();
}

void Roboute::readSettings()
{
    debug("Reading settings...");
    
    QSettings settings;
    
    QList<QVariant> list = settings.value("services/list").toList();
    
    QList<QVariant>::const_iterator beg = list.begin();
    QList<QVariant>::const_iterator end = list.end();
    
    for (; beg != end; ++beg) {
        addService(Service::fromSerialized(beg->toMap()));
    }
}

void Roboute::onServiceLaunched()
{
    Service* srv = static_cast<Service*>(sender());
    emit serviceLaunched(srv->id);
}

void Roboute::onServiceLaunching()
{
    Service* srv = static_cast<Service*>(sender());
    emit serviceLaunching(srv->id);
}

void Roboute::onServiceStopped()
{
    Service* srv = static_cast<Service*>(sender());
    emit serviceStopped(srv->id);
}

void Roboute::onServiceStopping()
{
    Service* srv = static_cast<Service*>(sender());
    emit serviceStopping(srv->id);
}

void Roboute::onAttributeChanged(const QString& key, const QString& value)
{
    Service* srv = static_cast<Service*>(sender());
    emit serviceAttrChange(srv->id, key, value);
}
