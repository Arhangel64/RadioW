#ifndef SSHSOCKET_H
#define SSHSOCKET_H

#include "qsshsocket.h"

#include <QtCore/QObject>
#include <QtCore/QThread>

namespace W {
    class SshSocket : public QObject {
        Q_OBJECT
        
    public:
        SshSocket(QObject* parent = 0);
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
        void authorize(const QString& login, const QString& password);
        void execute(const QString& command);
        
    signals:
        void opened();
        void closed();
        void authorized();
        void error(W::SshSocket::Error code, const QString& message);
        void data(const QString& command, const QString& data);
        void finished(const QString& command);
        
    private:
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
        State state;
        
    private slots:
        void onSocketConnected();
        void onSocketDisconnected();
        void onSocketLoggedIn();
        void onSocketError(QSshSocket::SshError p_error);
        void onSocketCommandData(QString command, QString p_data);
        void onSocketEOF(QString command);
        
    };
}

#endif // SSHSOCKET_H
