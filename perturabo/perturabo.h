#ifndef PERTURABO_H
#define PERTURABO_H

#include <QtCore/QObject>
#include <QtCore/QString>

#include <wSocket/socket.h>
#include <wSocket/server.h>

#include <wType/string.h>
#include <wType/uint64.h>
#include <wType/event.h>
#include <wType/address.h>
#include <wType/vocabulary.h>

#include <wDispatcher/dispatcher.h>
#include <wDispatcher/logger.h>

#include <wModel/modelstring.h>
#include <wModel/attributes.h>

#include <utils/exception.h>

#include <wServerUtils/commands.h>
#include <wServerUtils/connector.h>

#include "database.h"

class Perturabo: public QObject
{
    Q_OBJECT
    
public:
    Perturabo(QObject *parent = 0);
    ~Perturabo();
    
    static Perturabo* perturabo;
    
private:
    W::Server *server;
    W::Logger *logger;
    
    M::Attributes* attributes;
    U::Commands* commands;
    U::Connector* connector;
    
    Database* artists;
    Database* albums;
    Database* songs;
    
    handler(parseDirectory);
    
public:
    W::Dispatcher *dispatcher;
    
public slots:
    void start();
    void stop();
    
private slots:
    void onModelServiceMessage(const QString& msg);
    void onConnectionCountChanged(uint64_t count);
    
private:
    class SingletonError: 
        public Utils::Exception
    {
    public:
        SingletonError():Exception(){}
        
        std::string getMessage() const{return "Perturabo is a singleton, there was an attempt to construct it at the second time";}
    };
};

#endif // PERTURABO_H
