#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <utils/defines.h>

#include <QtCore/QObject>
#include <QtCore/QString>

#include <wType/address.h>
#include <wType/event.h>
#include <wType/vector.h>
#include <wType/vocabulary.h>
#include <wType/string.h>
#include <wDispatcher/dispatcher.h>
#include <wDispatcher/handler.h>
#include <wSocket/socket.h>
#include <wContainer/order.h>

namespace C {
    class Controller : public QObject
    {
        Q_OBJECT
    public:
        enum ModelType {
            string,
            list,
            vocabulary,
            
            attributes = 50
        };
        
        Controller(const W::Address& p_address, const W::Address& my_address, QObject* parent = 0);
        virtual ~Controller();
        
        void addController(C::Controller* ctrl);
        void addHandler(W::Handler* handler);
        void registerController(W::Dispatcher* dp, W::Socket* sock);
        void unregisterController();
        void subscribe();
        void unsubscribe();
        
        void removeHandler(W::Handler* handler);
        void removeController(C::Controller* ctrl);
        
        static C::Controller* createByType(int type, const W::Address& address, QObject* parent = 0);
        
    signals:
        void serviceMessage(const QString& msg);
        void modification(const W::Object& data);
        
    protected:
        W::Address pairAddress;
        W::Address address;
        
        void send(W::Vocabulary* vc, const W::Address& handlerAddress);
        handler(properties)
        
        void dropSubscribed();
        
    private:
        typedef W::Order<W::Handler*> HList;
        typedef W::Order<C::Controller*> CList;
        
        W::Dispatcher* dispatcher;
        W::Socket* socket;
        bool registered;
        bool subscribed;
        CList* controllers;
        HList* handlers;
        W::Vector* properties;
        
    protected slots:
        virtual void onSocketDisconnected();
    };
}


#endif // CONTROLLER_H
