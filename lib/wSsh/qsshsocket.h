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
    bool isExecuting();

signals:
    void connected();
    void disconnected();
    void error(QSshSocket::SshError error);
    void loginSuccessful();
    void commandData(QString data);
    void endOfFile();

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
    bool executing;
    Command* command;
    static bool lib_ssh_inited;
    static void lib_ssh_init();
    
private:
    void destroyCommand();
    
private slots:
    void socketRead(int ptr);
};


Q_DECLARE_METATYPE(QSshSocket::SshError)


#endif // QSSHSOCKET_H
