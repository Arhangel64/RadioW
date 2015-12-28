#ifndef SOCKET_H
#define SOCKET_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QtCore/QByteArray>

#include <wType/string.h>
#include <wType/uint64.h>
#include <wType/bytearray.h>
#include <wType/event.h>
#include <wType/vocabulary.h>

#include <utils/exception.h>

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
        
        void send(Event ev) const;
        
    private:
        explicit Socket(const String& p_name, QWebSocket *p_socket, QObject *parent = 0);
        
        void setHandlers();
        void setId(const Uint64& p_id);
        
        void setRemoteName();
        
        static ByteArray* QtoW(const QByteArray& in);
        static QByteArray* WtoQ(const ByteArray& in);
        
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
        
    private:
        class ErrorIdSetting: 
            public Utils::Exception
        {
        public:
            ErrorIdSetting():Exception(){}
            
            std::string getMessage() const{return "An attempt to set id to the socket not in connecting state";}
        };
        
    };
}

#endif // SOCKET_H
