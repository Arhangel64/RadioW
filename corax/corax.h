#ifndef CORAX_H
#define CORAX_H

#include <QtCore/QObject>

#include "libWSocket/wsocket.h"
#include "libWSocket/wserver.h"

class Corax: public QObject
{
	Q_OBJECT
	
public:
	Corax(QObject *parent = 0);
	
private:
	WServer *server;
    WSocket *socket;

public slots:
	void onNewConnection(WSocket *socket);
    void onSocketConnected();
    void onSocketEvent(const QString& msg);
    void onServerEvent(const QString& msg);
	
};

#endif // CORAX_H
