#ifndef WTESTSTRING_H
#define WTESTSTRING_H

#include "wevent.h"

class WTestString : public WEvent
{
public:
    WTestString(quint64 id_p);
    WTestString();
    
    QString text;
    
protected:
    virtual QDataStream& serialize(QDataStream& out) const;
    virtual QDataStream& deserialize(QDataStream& in);
};

#endif // WTESTSTRING_H
