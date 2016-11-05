#ifndef CORAX_H
#define CORAX_H

#include <QtCore/QObject>

#include <wSocket/socket.h>
#include <wSocket/server.h>

#include <wType/string.h>
#include <wType/uint64.h>
#include <wType/event.h>
#include <wType/address.h>
#include <wType/vocabulary.h>

#include <wDispatcher/dispatcher.h>
#include <wDispatcher/logger.h>
#include <wDispatcher/handler.h>

#include <wModels/modelstring.h>

#include <utils/exception.h>

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
    
    W::Handler* h_test;
    M::String modelName;
    
public:
    W::Dispatcher *dispatcher;
    
public:
    void test(const W::Event& ev);
    
public slots:
    void onNewConnection(const W::Socket& socket);
    void onSocketDisconnected();
    void start();
    void stop();
    
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
