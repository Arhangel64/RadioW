#ifndef W_MODEL_H
#define W_MODEL_H

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

#define handler(HANDLER)    \
    void _h_##HANDLER(const W::Event& ev) {h_##HANDLER(ev);}\
    virtual void h_##HANDLER(const W::Event& ev);\


namespace M {
    
    class Model : public QObject
    {
        Q_OBJECT
    public:
        Model(const W::Address p_address, QObject* parent = 0);
        //i'm not sure about copy constructor, it doesn't make sence
        virtual ~Model();
        
        void addModel(M::Model* model);
        void addHandler(W::Handler* handler);
        void addProperty(const W::String& value, const W::String& name);
        W::Address getAddress() const;
        void registerModel(W::Dispatcher* dp, W::Server* srv);
        void unregisterModel();
        
    signals:
        void serviceMessage(const QString& msg);
        
    protected:
        W::Address address;
        
        void response(W::Vocabulary* vc, const W::Address& handlerAddress, const W::Event& src);
        void broadcast(W::Vocabulary* vc, const W::Address& handlerAddress);
        
        handler(subscribe)
        handler(unsubscribe)
        
    private:
        typedef std::map<uint64_t, W::Order<W::Address>> Map;
        typedef std::list<W::Handler*> HList;
        typedef std::list<M::Model*> MList;
        
        bool registered;
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

