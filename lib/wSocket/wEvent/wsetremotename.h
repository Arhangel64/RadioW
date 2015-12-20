#ifndef WSETREMOTENAME_H
#define WSETREMOTENAME_H

#include "wevent.h"

class WSetRemoteName : public WEvent
{
public:
    WSetRemoteName(quint64 p_id, const QString&);
    WSetRemoteName();

    QString getName() const;

private:
    QString name;

protected:
    QDataStream & serialize(QDataStream &) const;
    QDataStream & deserialize(QDataStream &);
};


#endif // WSETREMOTENAME_H
