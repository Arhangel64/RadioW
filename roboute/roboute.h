#ifndef ROBOUTE_H
#define ROBOUTE_H

#include <QtCore/QObject>
#include <QMap>

#include <wType/string.h>
#include <wType/uint64.h>
#include <wType/event.h>
#include <wType/address.h>
#include <wType/vocabulary.h>

#include <wDispatcher/dispatcher.h>
#include <wDispatcher/logger.h>
#include <wDispatcher/handler.h>

#include <utils/exception.h>

#include <models/service.h>

class Roboute: public QObject
{
    Q_OBJECT
    
public:
    Roboute(QObject *parent = 0);
    ~Roboute();
    
    static Roboute* roboute;
    
private:
    W::Logger *logger;
    QMap<uint64_t, Service*> services;
    
public:
    W::Dispatcher *dispatcher;
    
public:
    
private:
    void debug(std::string str);
    
signals:
    void debugMessage(const QString& msg);
    void newService(const Service& srv);
    void serviceConnected(uint64_t);
    void serviceDisconnected(uint64_t);
    void serviceConnectionFailed(uint64_t);
    void serviceLaunched(uint64_t);
    void serviceStopped(uint64_t);
    void serviceLaunchingFailed(uint64_t);
    void serviceStoppingFailed(uint64_t);
    
public slots:
    void start();
    void stop();
    void addService(const QMap<QString, QString>& params);
    void connectService(uint64_t id);
    void disconnectService(uint64_t id);
    void launchService(uint64_t id);
    void stopService(uint64_t id);
    
private:
    class SingletonError: 
        public Utils::Exception
    {
    public:
        SingletonError():Exception(){}
        
        std::string getMessage() const{return "Roboute is a singleton, there was an attempt to construct it at the second time";}
    };
};

#endif // ROBOUTE_H
