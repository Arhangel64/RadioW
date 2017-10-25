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
        
    signals:
        void serviceMessage(const QString& msg);
        
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

    };

}


#endif // CONNECTOR_H
