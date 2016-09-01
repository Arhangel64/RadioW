#ifndef QSSHSOCKET_H
#define QSSHSOCKET_H

#include <libssh/libssh.h>
#include <QtCore/QByteArray>
#include <QtCore/QThread>
#include <QtCore/QVector>
#include <QtCore/QFile>
#include <sys/stat.h>
#include <sys/types.h>
#include <QtCore/QDebug>

class QSshSocket: public QThread
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

    explicit QSshSocket(QObject * parent = 0);
    ~QSshSocket();

    void connectToHost(QString host, int port =22);
    void disconnectFromHost();
    void executeCommand(QString command);

    QString host();
    bool isLoggedIn();
    bool isConnected();
    void login(QString user, QString password);
    int port();
    void pullFile(QString localPath, QString remotePath);
    void pushFile(QString localPath, QString remotePath);
    void setWorkingDirectory(QString path);
    QString user();

signals:

    void connected();
    void disconnected();
    void error(QSshSocket::SshError error);
    void commandExecuted(QString command,QString response);
    void loginSuccessful();
    void pullSuccessful(QString localFile, QString remoteFile);
    void pushSuccessful(QString localFile, QString remoteFile);
    void workingDirectorySet(QString cwd);

private slots:
    void run();

private:

    enum SSHOperationType
    {
        Command,
        WorkingDirectoryTest,
        Pull,
        Push
    };

    struct SSHOperation
    {
        SSHOperationType type;
        QString adminCommand,command, localPath, remotePath;
        bool executed;
    };

    int m_port;
    bool m_loggedIn ;
    QThread * m_thread;
    QString m_workingDirectory,m_nextWorkingDir,m_user, m_host,m_password;
    SSHOperation m_currentOperation;
    ssh_session m_session;
    bool m_connected,m_run;
};


#endif // QSSHSOCKET_H
