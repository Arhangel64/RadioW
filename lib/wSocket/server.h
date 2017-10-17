#ifndef SERVER_H
#define SERVER_H
#include <QtCore/QObject>
#include <QtWebSockets/QWebSocketServer>
#include <QtNetwork/QHostAddress>

#include <wType/string.h>

#include <stdint.h>
#include <map>
#include <set>

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
        
        const Socket& getConnection(uint64_t p_id) const;
        uint64_t getConnectionsCount() const;
        void closeConnection(uint64_t p_id);
        void openConnection(const String& addr, const Uint64& port);
        
    private:
        uint64_t lastId;
        std::set<uint64_t> pool;
        std::map<uint64_t, Socket*> connections;
        QWebSocketServer* server;
        String name;
        
        Socket* createSocket(QWebSocket* socket);
        
    signals:
        void newConnection(const W::Socket&);

    public slots:

    private slots:
        void onNewConnection();
        void onServerError(QWebSocketProtocol::CloseCode code);
        
        void onSocketConnected();
        void onSocketDisconnected();
        void onSocketNegotiationId(uint64_t p_id);
        
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
