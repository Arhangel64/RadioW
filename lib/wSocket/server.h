#ifndef SERVER_H
#define SERVER_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocketServer>
#include <QtNetwork/QHostAddress>

#include <wType/string.h>

#include <stdint.h>
#include <map>

#include "socket.h"

#include <utils/exception.h>

namespace W
{
    class Server:
        public QObject
    {
        Q_OBJECT
    public:
        explicit Server(const String& name, QObject *parent = 0);
        ~Server();
        
        void listen(uint16_t port);
        void stop();
        
        const Socket& getPeer(uint64_t p_id) const;
        
    private:
        typedef std::map<String, std::map<uint64_t, Socket*>> p_map;
        
        uint64_t lastId;
        std::map<uint64_t, Socket*> connections;
        p_map peers;
        QWebSocketServer* server;
        String name;
        
    signals:
        void newConnection(const W::Socket&);

    public slots:

    private slots:
        void onNewConnection();
        void onServerError(QWebSocketProtocol::CloseCode code);
        
        void onSocketConnected();
        void onSocketDisconnected();
        
    private:
        class HandshakeNameError: 
            public Utils::Exception
        {
        public:
            HandshakeNameError():Exception(){}
            
            std::string getMessage() const{return "Name of connected socket haven't been found, but registering returned an error";}
        };
    };
}

#endif // SERVER_H
