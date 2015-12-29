#ifndef CORAX_H
#define CORAX_H

#include <QtCore/QObject>

#include <wSocket/socket.h>
#include <wSocket/server.h>

#include <wType/string.h>
#include <wType/uint64.h>

class Corax: public QObject
{
    Q_OBJECT
    
public:
    Corax(QObject *parent = 0);
    
private:
    W::Server *server;
    W::Socket *socket;
    
public slots:
    void onNewConnection(const W::Socket& socket);
    void onSocketConnected();
};

#endif // CORAX_H
