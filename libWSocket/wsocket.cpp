#include "wsocket.h"

#include <QDebug>

WSocket::WSocket(const QString& p_name, QObject *parent):
    QObject(parent),
    id(0),
    serverCreated(false),
    state(Disconnected),
    socket(new QWebSocket()),
    name(p_name),
    remoteName()
{
    socket->setParent(this);
    setHandlers();
}

WSocket::WSocket(const QString& p_name, QWebSocket *p_socket, QObject *parent):
    QObject(parent),
    id(0),
    serverCreated(true),
    state(Connecting),
    socket(p_socket),
    name(p_name),
    remoteName()
{
    socket->setParent(this);
    setHandlers();
}

void WSocket::open(const QUrl& url)
{
    if (state == Disconnected) {
        state = Connecting;
        socket->open(url);
    }
}

void WSocket::close()
{
    if (state != Disconnected && state != Disconnecting) {
        state = Disconnecting;
        socket->close();
	emit t_ev(QString("socket closing"));
    }
}

void WSocket::setId(quint64 p_id) {
    if (id == 0 && state == Connecting) {
        id = p_id;
        WSetId* ev = new WSetId(id);
        socket->sendBinaryMessage(WEvent::serializeToQByteArray(ev));
        delete ev;
    }
}

void WSocket::setHandlers() {
    connect(socket, SIGNAL(connected()), SLOT(onSocketConnected()));
    connect(socket, SIGNAL(disconnected()), SLOT(onSocketDisconnected()));
    connect(socket, SIGNAL(binaryMessageReceived(const QByteArray&)), SLOT(onBinaryMessageReceived(const QByteArray&)));
}

void WSocket::onSocketConnected() {

}

void WSocket::onSocketDisconnected() {
    state = Disconnected;
    id = 0;
    emit t_ev(QString("socket closed"));
    emit disconnected();
}

void WSocket::onBinaryMessageReceived(const QByteArray& ba) {
    WEvent * ev = WEvent::createFromQByteArray(ba);
    switch(ev->getEventType())
    {
    case WEvent::tWEvent:
	break;    
    case WEvent::tWSetId:
        getRemoteId(static_cast<WSetId*>(ev));
        break;
    case WEvent::tWSetRemoteName:
        getRemoteName(static_cast<WSetRemoteName*>(ev));
        break;
    case WEvent::tWTestString:
	emit testString(*(static_cast<WTestString*>(ev)));
	break;
    }
    delete ev;
}

void WSocket::getRemoteId(WSetId* ev) {
    if (id == 0 && state == Connecting) {
	if (ev->getVersion() == WEvent::version) {
	    id = ev->getSenderId();
	    setRemoteName();
	} else {
	    emit t_ev(QString("Connecting socket protocol has incompatible"));
	    close();
	}
    } else {
        emit t_ev(QString("id has'nt been set"));
        //todo exeption
    }
}

void WSocket::finishHandshake() {
    if (serverCreated) {
	setRemoteName();
    }
    state = Connected;
    emit connected();
}

void WSocket::getRemoteName(WSetRemoteName* ev) {
    if (state == Connecting) {
        remoteName = ev->getName();
        finishHandshake();
    } else {
        emit t_ev(QString("not got the name"));
        //todo exeption
    }
}

void WSocket::setRemoteName() {
    if (state == Connecting) {
        WSetRemoteName* ev = new WSetRemoteName(id, name);
        socket->sendBinaryMessage(WEvent::serializeToQByteArray(ev));
        delete ev;
    }
}

quint64 WSocket::getId() const {
    return id;
}


void WSocket::send(WEvent* ev)
{
    socket->sendBinaryMessage(WEvent::serializeToQByteArray(ev));
}
