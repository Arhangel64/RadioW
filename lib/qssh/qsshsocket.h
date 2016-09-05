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
        SocketError,                        //There was trouble creating a socket. This was most likely due to the lack of an internet connection.
        SessionCreationError,               //The ssh session could not be created due to inability to find the remote host
        ChannelCreationError,               //An ssh channel could not be created for the previous operation
        ScpChannelCreationError,            //An scp channel could not be created for the previous file transfer operation
        ScpPullRequestError,                //There was an error requesting a pull file transfer
        ScpPushRequestError,                //There was an error requesting a push file transfer
        ScpFileNotCreatedError,             //The destination file for the previous transfer does not exist
        ScpReadError,                       //There was an error reading a remote file. This could possibly be due to user permissions
        ScpWriteError,                      //There was an error writing to a remote file. This could possibly be due to user permissions
        PasswordAuthenticationFailedError   //The credentials of a user on the remote host could not be authenticated
    };

    explicit QSshSocket(QObject* parent = 0);
    ~QSshSocket();

    void connect(QString host, int port = 22);
    void disconnect();
    void executeCommand(QString command);
    void login(QString user, QString password);

    bool isLoggedIn();
    bool isConnected();

signals:
    void connected();
    void disconnected();
    void error(QSshSocket::SshError error);
    void commandExecuted(QString response);
    void loginSuccessful();
    void commandData(QString command, QString data);


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
