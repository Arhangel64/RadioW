#ifndef SERVICE_H
#define SERVICE_H

#include <list>

#include <wSocket/socket.h>
#include <wSsh/sshsocket.h>
#include <wType/string.h>
#include <wType/uint64.h>

#include <wController/attributes.h>

#include <QtCore/QString>
#include <QtCore/QStringList>
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
    void registerContollers(W::Dispatcher* dp);
    void unregisterControllers(W::Dispatcher* dp);
    
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
        Unknown,
        Checking,
        Dead,
        Launching,
        WaitingWebSocket,
        Active,
        Stopping
    };
    
    W::Socket* socket;
    W::SshSocket* dataSsh;
    W::SshSocket* commandSsh;
    C::Attributes* attributes;
    static uint64_t lastId;
    QString login;
    QString password;
    QString logFile;
    QString command;
    std::list<QString> psResults;
    QString pid;
    State state;
    AppState appState;
    
    void requestPid();
    void connectWebsocket();
    
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
    void attributeChanged(const QString& name, const QString& value);
    
public slots:
    void connect();
    void disconnect();
    void launch();
    void stop();
    
private:
    
private slots:
    void onDataSshOpened();
    void onCommandSshOpened();
    void onSshClosed();
    void onDataSshData(const QString& data);
    void onCommandSshData(const QString& data);
    void onSshError(W::SshSocket::Error errCode, const QString& msg);
    void onDataSshFinished();
    void onCommandSshFinished();
    
    void onSocketConnected();
    void onSocketDisconnected();
    void onSocketError(W::Socket::SocketError err, const QString& msg);
    void onAttrChange(const W::String& key, const W::Object& value);
};

#endif // SERVICE_H
