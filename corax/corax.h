#ifndef CORAX_H
#define CORAX_H

#include <QtCore/QObject>

//#include <wSocket/wsocket.h>
//#include <wSocket/wserver.h>

#include <wserver.h>
#include <wsocket.h>

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
