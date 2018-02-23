#ifndef CORAX_H
#define CORAX_H

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

#include <wServerUtils/commands.h>
#include <wServerUtils/connector.h>

#include <wDatabase/resourcecache.h>

#include "tools/parser.h"

class Corax: public QObject
{
    Q_OBJECT
    
public:
    Corax(QObject *parent = 0);
    ~Corax();
    
    static Corax* corax;
    
private:
    W::Server *server;
    W::Logger *logger;
    W::ParentReporter* parentReporter;
    
    M::Attributes* attributes;
    U::Commands* commands;
    U::Connector* connector;
    W::Dispatcher *dispatcher;
    
    std::map<W::String, ResourceCache*> caches;
    std::map<W::String, Parser*> parsers;
    
    handler(clearCache);
    handler(parseDirectory);
    
public slots:
    void start();
    void stop();
    
private slots:
    void onModelServiceMessage(const QString& msg);
    void onConnectionCountChanged(uint64_t count);
    void onParserDone(const W::String& path);
    void onCacheCountChange(uint64_t count);
    void onNodeConnected(const W::String& name);
    void onNodeDisconnected(const W::String& name);
    
private:
    void addCache(ResourceCache* cache);
    void createCaches();
    void createHandlers();
    
private:
    class SingletonError: 
        public Utils::Exception
    {
    public:
        SingletonError():Exception(){}
        
        std::string getMessage() const{return "Corax is a singleton, there was an attempt to construct it at the second time";}
    };
};

#endif // CORAX_H
