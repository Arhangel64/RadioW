#ifndef PERTURABO_H
#define PERTURABO_H

#include <map>

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
#include <wDispatcher/parentreporter.h>

#include <wModel/modelstring.h>
#include <wModel/attributes.h>

#include <utils/exception.h>
#include <tools/file.h>

#include <wServerUtils/commands.h>
#include <wServerUtils/connector.h>

#include <wDatabase/database.h>

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
    W::ParentReporter* parentReporter;
    
    M::Attributes* attributes;
    U::Commands* commands;
    U::Connector* connector;
    
    std::map<W::String, Database*> databases;
    
    handler(clearDatabase);
//     handler(parseDirectory);
    
public:
    W::Dispatcher *dispatcher;
    
public slots:
    void start();
    void stop();
    
private slots:
    void onModelServiceMessage(const QString& msg);
    void onConnectionCountChanged(uint64_t count);
    void onDatabaseCountChange(uint64_t count);
    
private:
    void createDatabases();
    void addDatabase(Database* db);
    
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
