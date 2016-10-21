#ifndef SSHSOCKET_H
#define SSHSOCKET_H

#include "qsshsocket.h"

#include <QtCore/QObject>
#include <QtCore/QThread>

namespace W {
    class SshSocket : public QObject {
        Q_OBJECT
        
    public:
        SshSocket(const QString& p_login, const QString& p_password, QObject* parent = 0);
        ~SshSocket();
        
        enum Error {
            SocketError,
            SessionCreationError,
            ChannelCreationError,
            ReadError,
            WriteError,
            PasswordAuthenticationError
        };
        
        void open(const QString& address, uint16_t port = 22);
        void close();
        void execute(const QString& command);
        bool isReady() const;
        
    signals:
        void opened();
        void closed();
        void error(W::SshSocket::Error code, const QString& message);
        void data(const QString& data);
        void finished();
        
    private:
        void authorize();
        enum State {
            Disconnected,
            Connecting,
            Connected,
            Authorizing,
            Authorized,
            Disconnecting
        };
        
        QSshSocket* socket;
        QThread* thread;
        QString login;
        QString password;
        State state;
        
    private slots:
        void onSocketConnected();
        void onSocketDisconnected();
        void onSocketLoggedIn();
        void onSocketError(QSshSocket::SshError p_error);
        void onSocketCommandData(QString p_data);
        void onSocketEOF();
        
    };
}

#endif // SSHSOCKET_H
