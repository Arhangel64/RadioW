#ifndef ROBOUTE_H
#define ROBOUTE_H

#include <QtCore/QObject>

#include <wSocket/socket.h>

#include <wType/string.h>
#include <wType/uint64.h>
#include <wType/event.h>
#include <wType/address.h>
#include <wType/vocabulary.h>

#include <wDispatcher/dispatcher.h>
#include <wDispatcher/logger.h>
#include <wDispatcher/handler.h>

#include <utils/exception.h>

class Roboute: public QObject
{
    Q_OBJECT
    
public:
    Roboute(QObject *parent = 0);
    ~Roboute();
    
    static Roboute* roboute;
    
private:
    W::Logger *logger;
    
public:
    W::Dispatcher *dispatcher;
    
public:
    
private:
    void debug(std::string str);
    
signals:
    void debugMessage(const QString& msg);
    
public slots:
    void start();
    void stop();
    
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
