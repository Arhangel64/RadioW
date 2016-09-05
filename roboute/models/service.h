#ifndef SERVICE_H
#define SERVICE_H

#include <wSocket/socket.h>
#include <wSsh/sshsocket.h>
#include <wType/string.h>
#include <QtCore/QString>
#include <QtCore/QMap>

class Service : public QObject
{
    Q_OBJECT
private:
    Service(uint64_t p_id, const QString& p_name, const QString& p_address, const QString& p_port, const QString& p_login, const QString& p_password);
    
public:
    ~Service();
    
    static Service* create(const QMap<QString, QString>& params);
    
private:
    W::Socket* socket;
    W::SshSocket* dataSsh;
    static uint64_t lastId;
    QString login;
    QString password;
    
public:
    QString name;
    QString address;
    QString port;
    const uint64_t id;
    
signals:
    void serviceMessage(const QString&);
    
    
public slots:
    void connect();
    void disconnect();
    
private:
    
private slots:
    void onSshOpened();
    void onSshClosed();
    void onSshAuthorized();
    void onSshData(const QString& command, const QString& data);
    void onSshError(W::SshSocket::Error errCode, const QString& msg);
    
};

#endif // SERVICE_H
