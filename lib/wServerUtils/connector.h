#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QtCore/QObject>
#include <map>
#include <set>

#include <wDispatcher/dispatcher.h>

#include <wSocket/socket.h>
#include <wSocket/server.h>

#include <wType/string.h>
#include <wType/uint64.h>
#include <wType/event.h>

#include <utils/exception.h>

#include "commands.h"

namespace U {
    class Connector : public QObject
    {
        Q_OBJECT
        typedef std::map<W::String, uint64_t> Map;
    public:
        Connector(W::Dispatcher* dp, W::Server* srv, Commands* cmds, QObject* parent = 0);
        ~Connector();
        
        void addIgnoredNode(const W::String& name);
        void sendTo(const W::String& name, const W::Event& event);
        const W::Socket& getNodeSocket(const W::String& name);
        
    signals:
        void serviceMessage(const QString& msg);
        void nodeConnected(const W::String& name);
        void nodeDisconnected(const W::String& name);
        
    private:
        W::Dispatcher* dispatcher;
        W::Server* server;
        U::Commands* commands;
        Map nodes;
        std::set<W::String> ignoredNodes;
        
    protected:
        handler(connect);
        handler(disconnect);
        
    private slots:
        void onNewConnection(const W::Socket& socket);
        void onClosedConnection(const W::Socket& socket);

    public:
        class NodeAccessError: 
        public Utils::Exception
        {
            W::String name;
        public:
            NodeAccessError(const W::String& p_name):Exception(), name(p_name){}
            
            std::string getMessage() const{return std::string("An attempt to access non existing node ") + name.toString();}
        };
    };
}


#endif // CONNECTOR_H
