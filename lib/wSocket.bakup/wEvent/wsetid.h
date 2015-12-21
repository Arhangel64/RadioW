#ifndef WSETID_H
#define WSETID_H

#include "wevent.h"

class WSetId : public WEvent
{
public:
    WSetId(quint64 id_p);
    WSetId();

    QString getVersion() const;

private:
    QString version;

protected:
    QDataStream & serialize(QDataStream &) const;
    QDataStream & deserialize(QDataStream &);
};

#endif // WSETID_H
