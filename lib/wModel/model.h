#ifndef W_MODEL_H
#define W_MODEL_H

#include <utils/defines.h>

#include <map>
#include <list>
#include <QtCore/QObject>
#include <QtCore/QString>

#include <wType/address.h>
#include <wType/vector.h>
#include <wType/event.h>
#include <wType/string.h>
#include <wSocket/socket.h>
#include <wDispatcher/handler.h>
#include <wContainer/order.h>
#include <wServerUtils/connector.h>
#include <wController/controller.h>

namespace M {
    
    class Model : public QObject
    {
        Q_OBJECT
    public:
        enum ModelType {
            string,
            list,
            vocabulary,
            catalogue,
            image,
            button,
            model,
            
            attributes = 50,
            file,
            resourceCache,
            
            player = 107
        };
        
        Model(const W::Address p_address, QObject* parent = 0);
        //i'm not sure about copy constructor, it just doesn't make sense, because the address is the parameter which is supposed to be unique
        virtual ~Model();
        
        virtual ModelType getType() const = 0;
        virtual void set(W::Object* value) = 0;
        virtual void set(const W::Object& value) = 0;
        
        void addModel(M::Model* model);
        void addHandler(W::Handler* handler);
        void addProperty(const W::String& value, const W::String& name);
        void addController(C::Controller* ctrl, const W::String& nodeName);
        W::Address getAddress() const;
        void getRegistered(U::Connector* connector);
        void getUnregistered();
        
        void removeHandler(W::Handler* handler);
        void removeModel(M::Model* model);
        void removeController(C::Controller* ctrl);
        void passToLocalHandler(const W::Event& event) const;
        
    signals:
        void serviceMessage(const QString& msg) const;
        void subscribersCountChange(uint64_t count) const;
        
    protected:
        typedef std::map<W::Address, W::Vocabulary> SMap;
        typedef std::map<uint64_t, SMap> Map;
        W::Address address;
        bool registered;
        Map* subscribers;
        
        void send(W::Vocabulary* vc, const W::Address& destination, uint64_t connectionId);
        void response(W::Vocabulary* vc, const W::Address& handlerAddress, const W::Event& src);
        void fakeResponse(W::Vocabulary* vc, const W::Address& handlerAddress, const W::Address& sourceAddress, const W::Event& src);
        void broadcast(W::Vocabulary* vc, const W::Address& handlerAddress);
        
        handler(subscribe)
        handler(unsubscribe)
        
    private:
        typedef W::Order<W::Handler*> HList;
        typedef W::Order<M::Model*> MList;
        typedef std::map<C::Controller*, W::String> Controllers;
        
        U::Connector* connector;
        uint64_t subscribersCount;
        HList* handlers;
        W::Vector* properties;
        MList* models;
        Controllers* controllers;
        
    private slots:
        void onSocketDisconnected();
    };
}
#endif // W_MODEL_H

