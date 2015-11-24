#include "wevent.h"

#include <QBuffer>
#include <iostream>

#include "wsetid.h"
#include "wsetremotename.h"

using std::cout;
using std::endl;

QString WEvent::version("0.0.1");

WEvent::WEvent(quint64 senderId_p, typeId eventType_p):
    eventType(eventType_p),
    senderId(senderId_p)
{
}

WEvent::WEvent(typeId eventType_p):
    eventType(eventType_p),
    senderId(0)
{
}

WEvent::typeId WEvent::getEventType() const
{
    return eventType;
}

quint64 WEvent::getSenderId() const
{
    return senderId;
}

QDataStream & WEvent::serialize(QDataStream& out) const
{
    out << eventType << senderId;
    return out;
}

QDataStream & WEvent::deserialize(QDataStream& in)
{
    in >> senderId;
    return in;
}

WEvent* WEvent::createFromQByteArray(const QByteArray & ba)
{
    QDataStream ds(ba);
    WEvent *ret;
    int id;
    ds >> id;
    switch (id) {
    case WEvent::tWEvent:
        ret = new WEvent();
        break;
    case WEvent::tWSetId:
        ret = new WSetId();
        break;
    case WEvent::tWSetRemoteName:
        ret = new WSetRemoteName();
        break;
    }
    ret->deserialize(ds);
    return ret;
}

QByteArray WEvent::serializeToQByteArray(WEvent* ev)
{
    QByteArray ba;
    QBuffer bufo(&ba);
    bufo.open(QIODevice::WriteOnly);
    QDataStream ds(&bufo);
    ev->serialize(ds);
    bufo.close();
    return ba;
}

