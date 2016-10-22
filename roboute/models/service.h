#ifndef SERVICE_H
#define SERVICE_H

#include <wSocket/socket.h>
#include <wSsh/sshsocket.h>
#include <wType/string.h>
#include <wType/uint64.h>

#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QVariant>

class Service : public QObject
{
    Q_OBJECT
private:
    Service(uint64_t p_id, 
            const QString& p_name, 
            const QString& p_address, 
            const QString& p_port, 
            const QString& p_login, 
            const QString& p_password, 
            const QString& p_logFile, 
            const QString& p_command
    );
    
public:
    ~Service();
    
    static Service* create(const QMap<QString, QString>& params);
    static Service* fromSerialized(const QMap<QString, QVariant>& params);
    QVariant saveState() const;
    
private:
    enum State {
        Disconnected,
        Connecting,
        Echo,
        Listening,
        Connected,
        Disconnecting
    };
    
    enum AppState {
        Dead,
        Checking,
        Launching,
        WaitingWebSocket,
        Active,
        Stopping
    };
    
    W::Socket* socket;
    W::SshSocket* dataSsh;
    W::SshSocket* commandSsh;
    static uint64_t lastId;
    QString login;
    QString password;
    QString logFile;
    QString command;
    State state;
    AppState appState;
    
public:
    QString name;
    QString address;
    QString port;
    const uint64_t id;
    
signals:
    void serviceMessage(const QString& msg);
    void connecting();
    void connected();
    void disconnecting();
    void disconnected();
    void launching();
    void launched();
    void stopping();
    void stopped();
    void log(const QString& data);
    
public slots:
    void connect();
    void disconnect();
    void launch();
    void stop();
    
private:
    
private slots:
    void onSshOpened();
    void onSshClosed();
    void onDataSshData(const QString& data);
    void onCommandSshData(const QString& data);
    void onSshError(W::SshSocket::Error errCode, const QString& msg);
    void onDataSshFinished();
    void onCommandSshFinished();
    
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketError(W::Socket::SocketError err, const QString& msg);
};

#endif // SERVICE_H
