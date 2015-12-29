#ifndef SERVER_H
#define SERVER_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocketServer>
#include <QtNetwork/QHostAddress>

#include <wType/string.h>

#include <stdint.h>
#include <map>

#include "socket.h"

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
        
    private:
        uint64_t lastId;
        std::map<uint64_t, Socket*> connections;
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
    };
}

#endif // SERVER_H
