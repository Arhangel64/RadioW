#include "wsetid.h"

WSetId::WSetId(quint64 id_p):
    WEvent(id_p, WEvent::tWSetId),
    version(WEvent::version)
{

}

WSetId::WSetId():
    WEvent(WEvent::tWSetId),
    version()
{

}

QString WSetId::getVersion() const {
    return version;
}

QDataStream & WSetId::serialize(QDataStream & out) const {
    WEvent::serialize(out) << version;
    return out;
}

QDataStream & WSetId::deserialize(QDataStream & in) {
    WEvent::deserialize(in) >> version;
    return in;
}
