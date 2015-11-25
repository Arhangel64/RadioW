#ifndef WEVENT_H
#define WEVENT_H

#include <QtCore/QByteArray>
#include <QtCore/QDataStream>


class WEvent
{

public:
    enum typeId
    {
        tWEvent,
        tWSetId,
        tWSetRemoteName,
	tWTestString
    };
    WEvent(quint64 senderId_p, typeId eventType_p = tWEvent);
    WEvent(typeId eventType_p = tWEvent);
    virtual ~WEvent() {}

    typeId getEventType() const;
    quint64 getSenderId() const;

    static WEvent* createFromQByteArray(const QByteArray&);
    static QByteArray serializeToQByteArray(WEvent*);

    static QString version;

protected:
    virtual QDataStream& serialize(QDataStream&) const;
    virtual QDataStream& deserialize(QDataStream&);

private:
    typeId eventType;
    quint64 senderId;
};

#endif // WEVENT_H
