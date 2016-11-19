#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <list>
#include <QtCore/QObject>
#include <QtCore/QString>

#include <wType/address.h>
#include <wType/event.h>
#include <wType/vector.h>
#include <wType/vocabulary.h>
#include <wDispatcher/dispatcher.h>
#include <wDispatcher/handler.h>
#include <wSocket/socket.h>

namespace C {
    class Controller : public QObject
    {
        Q_OBJECT
    public:
        Controller(const W::Address p_address, QObject* parent = 0);
        virtual ~Controller();
        
        void addController(C::Controller* ctrl);
        void addHandler(W::Handler* handler);
        void registerController(W::Dispatcher* dp, W::Socket* sock);
        void unregisterController();
        void subscribe();
        void unsubscribe();
        
    signals:
        void serviceMessage(const QString& msg);
        
    protected:
        W::Address pairAddress;
        W::Address address;
        
        void send(W::Vocabulary* vc, const W::Address& handlerAddress);
        virtual void h_properties(const W::Event& event);
        
        
    private:
        typedef std::list<W::Handler*> HList;
        typedef std::list<C::Controller*> CList;
        
        W::Dispatcher* dispatcher;
        W::Socket* socket;
        bool registered;
        bool subscribed;
        CList* controllers;
        HList* handlers;
        W::Vector* properties;
    };
}


#endif // CONTROLLER_H
