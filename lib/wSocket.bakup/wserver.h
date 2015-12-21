#ifndef WSERVER_H
#define WSERVER_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocketServer>
#include <QtNetwork/QHostAddress>
#include <QtWebSockets/QWebSocket>
#include <QtCore/QHash>

#include "wsocket.h"

class WServer : public QObject
{
    Q_OBJECT

    quint64 lastId;

public:
    explicit WServer(const QString& name = QString(), QObject *parent = 0);

    void listen(const QHostAddress& address = QHostAddress::Any, quint16 port = 0);
    void close();

    QHash <quint64, WSocket*> connections;

private:
    QWebSocketServer* server;
    QString name;

signals:
    void newConnection(WSocket* socket);

public slots:

private slots:
    void onNewConnection();
    void onSocketConnected();
    void onSocketDisconnected();
};

#endif // WSERVER_H
