#ifndef SOCKET_H
#define SOCKET_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtCore/QByteArray>

#include <wType/string.h>
#include <wType/uint64.h>

namespace W
{
    class Socket:
        public QObject
    {
        Q_OBJECT
        friend class WServer;
        
        enum State
        {
            disconnected_s,
            disconnecting_s,
            connecting_s,
            connected_s
        };
        
    public:
        explicit Socket(const String& p_name, QObject* parent = 0);
        ~Socket();
        
    private:
        explicit Socket(const String& p_name, QWebSocket *p_socket, QObject *parent = 0);
        
        void setHandlers();
        
        bool serverCreated;
        State state;
        QWebSocket *socket;
        Uint64 id;
        String name;
        String remoteName;
        
    signals:
        void disconnected();
        
    private slots:
        void onSocketConnected();
        void onSocketDisconnected();
        void onBinaryMessageReceived(const QByteArray& ba);
        
    };
}

#endif // SOCKET_H
