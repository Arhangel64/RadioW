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
        friend class Server;
        
        enum State
        {
            disconnected_s,
            disconnecting_s,
            connecting_s,
            connected_s
        };
        
        enum DeserializationState {
            dSize,
            dBody
        };
        
    public:
        explicit Socket(const String& p_name, QObject* parent = 0);
        ~Socket();
        
        void send(const Event& ev) const;
        void open(const String& addr, const Uint64& port);
        void close();
        
        Uint64 getId() const;
        String getRemoteName() const;
        String getName() const;
        typedef QAbstractSocket::SocketError SocketError;
        
    private:
        explicit Socket(const String& p_name, QWebSocket *p_socket, uint64_t p_id, QObject *parent = 0);
        
        void setHandlers();
        void setId(const Uint64& p_id);
        void setRemoteId();
        void setRemoteName();
        void setName(const String& p_name);
        
        bool serverCreated;
        State state;
        DeserializationState dState;
        QWebSocket *socket;
        Uint64 id;
        String name;
        String remoteName;
        ByteArray* helperBuffer;
        
    signals:
        void connected();
        void disconnected();
        void negotiationId(uint64_t p_id);
        void error(W::Socket::SocketError err, const QString& msg);
        void message(const W::Event&);
        void proxy(const W::Event&);
        
    public slots:
        void cantDeliver(const Event& event) const;
        
    private slots:
        void onSocketConnected();
        void onSocketDisconnected();
        void onSocketError(QAbstractSocket::SocketError err);
        void onBinaryMessageReceived(const QByteArray& ba);
        void onEvent(W::Event* ev);
        
    private:
        class ErrorIdSetting: 
            public Utils::Exception
        {
        public:
            ErrorIdSetting():Exception(){}
            
            std::string getMessage() const{return "An attempt to set id to the socket not in connecting state";}
        };
        
        class ErrorNameSetting: 
            public Utils::Exception
        {
        public:
            ErrorNameSetting():Exception(){}
            
            std::string getMessage() const{return "An attempt to set name to the socket not in connecting state";}
        };
        
    };
}

#endif // SOCKET_H
