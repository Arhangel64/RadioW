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
#include <wSocket/server.h>
#include <wDispatcher/dispatcher.h>
#include <wDispatcher/handler.h>
#include <wContainer/order.h>

namespace M {
    
    class Model : public QObject
    {
        Q_OBJECT
    public:
        enum ModelType {
            string,
            list,
            vocabulary,
            
            attributes = 50
        };
        
        Model(const W::Address p_address, QObject* parent = 0);
        //i'm not sure about copy constructor, it just doesn't make sence, because the address in parameter supposed to be unique
        virtual ~Model();
        
        virtual ModelType getType() const = 0;
        virtual void set(W::Object* value) = 0;
        virtual void set(const W::Object& value) = 0;
        
        void addModel(M::Model* model);
        void addHandler(W::Handler* handler);
        void addProperty(const W::String& value, const W::String& name);
        W::Address getAddress() const;
        void registerModel(W::Dispatcher* dp, W::Server* srv);
        void unregisterModel();
        
        void removeHandler(W::Handler* handler);
        void removeModel(M::Model* model);
        
    signals:
        void serviceMessage(const QString& msg);
        void subscribersCountChange(uint64_t count);
        
    protected:
        W::Address address;
        bool registered;
        
        void response(W::Vocabulary* vc, const W::Address& handlerAddress, const W::Event& src);
        void broadcast(W::Vocabulary* vc, const W::Address& handlerAddress);
        
        handler(subscribe)
        handler(unsubscribe)
        
    private:
        typedef std::map<uint64_t, W::Order<W::Address>> Map;
        typedef W::Order<W::Handler*> HList;
        typedef W::Order<M::Model*> MList;
        
        W::Dispatcher* dispatcher;
        W::Server* server;
        Map* subscribers;
        uint64_t subscribersCount;
        HList* handlers;
        W::Vector* properties;
        MList* models;
        
    private slots:
        void onSocketDisconnected();
    };
}
#endif // W_MODEL_H

