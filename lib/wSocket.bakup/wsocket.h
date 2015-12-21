#ifndef WSOCKET_H
#define WSOCKET_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QByteArray>
#include <QtWebSockets/QWebSocket>

#include "wevent.h"
#include "wsetid.h"
#include "wsetremotename.h"
#include "wteststring.h"

class WSetId;
class WSetRemoteName;

enum State
{
    Disconnected,
    Disconnecting,
    Connecting,
    Connected
};

class WSocket: public QObject
{
    Q_OBJECT
    friend class WServer;
public:
    explicit WSocket(const QString& p_name, QObject *parent = 0);
    

    void open(const QUrl& url);
    void close();
    void send(WEvent* ev);

    quint64 getId() const;

private:
    explicit WSocket(const QString& p_name, QWebSocket *p_socket, QObject *parent = 0);
    
    void setId(quint64 p_id);
    void setRemoteName();
    void setHandlers();
    void getRemoteId(WSetId* ev);
    void getRemoteName(WSetRemoteName* ev);
    void finishHandshake();

    quint64 id;
    bool serverCreated;
    State state;
    QWebSocket* socket;
    QString name;
    QString remoteName;

signals:
    void connected();
    void disconnected();
    void t_ev(const QString&);
    
    void testString(WTestString);

private slots:
    void onSocketConnected();
    void onSocketDisconnected();
    void onBinaryMessageReceived(const QByteArray& ba);

};

#endif // WSOCKET_H
