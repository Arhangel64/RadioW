#include "wteststring.h"

WTestString::WTestString(quint64 id_p):
    WEvent(id_p, WEvent::tWTestString),
    text("")
{

}

WTestString::WTestString():
    WEvent(WEvent::tWTestString),
    text("")
{

}

QDataStream& WTestString::serialize(QDataStream& out) const
{
    WEvent::serialize(out) << text;
    return out;
}

QDataStream& WTestString::deserialize(QDataStream& in)
{
    WEvent::deserialize(in) >> text;
    return in;
}
