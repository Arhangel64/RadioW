#ifndef ROBOUTE_H
#define ROBOUTE_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtCore/QThread>
#include <QtCore/QSettings>

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
    void debug(std::string str) const;
    void debug(uint64_t id, const QString& msg) const;
    void saveSettings() const;
    void readSettings();
    void addService(Service* srv);
    
signals:
    void debugMessage(const QString& msg) const;
    void log(uint64_t id,const QString& msg);
    void newService(uint64_t id, const QString& name);
    void serviceChangeName(uint64_t id, const QString& name);
    void serviceAttrChange(uint64_t id, const QString& key, const QString& value);
    void serviceRemoved(uint64_t id);
    void serviceConnected(uint64_t id);
    void serviceConnecting(uint64_t id);
    void serviceDisconnecting(uint64_t id);
    void serviceDisconnected(uint64_t id);
    void serviceConnectionFailed(uint64_t id);
    void serviceLaunched(uint64_t id);
    void serviceStopped(uint64_t id);
    void serviceLaunching(uint64_t id);
    void serviceStopping(uint64_t id);
    void serviceEdit(uint64_t id, const QMap<QString, QString>& params);
    void serviceAddCommand(uint64_t id, const QString& key, const QMap<QString, uint64_t>& arguments);
    void serviceRemoveCommand(uint64_t id, const QString& key);
    void serviceClearCommands(uint64_t id);
    
    
public slots:
    void start();
    void stop();
    void addService(const QMap<QString, QString>& params);
    void changeService(uint64_t id, const QMap<QString, QString>& params);
    void removeService(uint64_t id);
    void connectService(uint64_t id);
    void disconnectService(uint64_t id);
    void launchService(uint64_t id);
    void editService(uint64_t id);
    void stopService(uint64_t id);
    void launchCommand(uint64_t id, const QString& name, const QMap<QString, QVariant>& args);
    
private slots:
    void onServiceMessage(const QString& msg);
    void onServiceChangeName(const QString& name);
    void onServiceConnecting();
    void onServiceConnected();
    void onServiceDisconnecting();
    void onServiceDisconnected();
    void onServiceLaunching();
    void onServiceLaunched();
    void onServiceStopping();
    void onServiceStopped();
    void onServiceLog(const QString& msg);
    void onAttributeChanged(const QString& key, const QString& value);
    void onAddCommand(const QString& key, const QMap<QString, uint64_t>& arguments);
    void onRemoveCommand(const QString& key);
    void onClearCommands();
    
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
