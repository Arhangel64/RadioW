#ifndef QSSHSOCKET_H
#define QSSHSOCKET_H

#include <libssh/libssh.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QSocketNotifier>

#include <QtCore/QByteArray>
#include <QtCore/QVector>
#include <QtCore/QMap>

class QSshSocket: public QObject
{
    Q_OBJECT
public:

    enum SshError
    {
        SocketError,
        SessionCreationError,
        ChannelCreationError,
        ReadError,
        WriteError,
        PasswordAuthenticationFailedError
    };

    explicit QSshSocket(QObject* parent = 0);
    ~QSshSocket();

    bool isLoggedIn();
    bool isConnected();

signals:
    void connected();
    void disconnected();
    void error(QSshSocket::SshError error);
    void loginSuccessful();
    void commandData(QString command, QString data);

public slots:
    void connect(QString host, int port = 22);
    void disconnect();
    void executeCommand(QString command);
    void login(QString user, QString password);

private:
    struct Command
    {
        qintptr id;
        QString command;
        ssh_channel channel;
        QSocketNotifier* notifier;
    };
    
    bool loggedIn;
    ssh_session session;
    bool m_connected;
    QMap<qintptr, Command*> commands;
    
private:
    void destroyCommand(quintptr ptr);
    
private slots:
    void socketRead(int ptr);
};


#endif // QSSHSOCKET_H
