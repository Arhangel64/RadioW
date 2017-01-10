#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>
#include <map>

#include <wDispatcher/dispatcher.h>

#include <wSocket/socket.h>
#include <wSocket/server.h>

#include <wType/string.h>
#include <wType/uint64.h>
#include <wType/event.h>

#include <commands.h>


namespace P {       //metaobject features are not supported in nested classes, so, that's just a way to hide it
    class Node : public QObject
    {
        Q_OBJECT
    public:
        Node(const W::String& p_name);
        W::Socket* socket;
        W::String name;
        bool connected;
        bool outgoing;
        bool pending;
        handler(connect);
        handler(disconnect);
        
    signals:
        void connect(const W::String& node, const W::String& addr, const W::Uint64& port);
        void disconnect(const W::String& node);
    };
}

class Connector : public QObject
{
    Q_OBJECT
    typedef std::map<W::String, P::Node*> Map;
    typedef std::map<W::Socket*, P::Node*> Voc;
public:
    Connector(const W::String& pnk, W::Dispatcher* dp, W::Server* srv, Commands* cmds, QObject* parent = 0);
    ~Connector();
    
    void addNode(const W::String& name);
    void sendTo(const W::String& name, const W::Event& event);
    uint64_t getConnectionCount() const;
    
signals:
    void connectionCountChange(uint64_t count);
    void serviceMessage(const QString& msg);
    
public slots:
    void connectTo(const W::String& name, const W::String& address, const W::Uint64& port);
    void disconnectNode(const W::String& name);
    
private:
    
    W::String parentNodeKey;
    W::Dispatcher* dispatcher;
    W::Server* server;
    Commands* commands;
    Map nodes;
    Voc pending;
    uint64_t occ;
    
private slots:
    void onNewConnection(const W::Socket& socket);
    void onSocketDisconnected();
    void outgoingSocketError(W::Socket::SocketError err, const QString& msg);
    void outgoingSocketConnected();

};


#endif // CONNECTOR_H
