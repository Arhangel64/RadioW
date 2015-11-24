#include "wsetremotename.h"

WSetRemoteName::WSetRemoteName(quint64 id_p, const QString& p_name):
    WEvent(id_p, WEvent::tWSetRemoteName),
    name(p_name)
{

}

WSetRemoteName::WSetRemoteName():
    WEvent(WEvent::tWSetRemoteName),
    name()
{

}

QString WSetRemoteName::getName() const {
    return name;
}

QDataStream & WSetRemoteName::serialize(QDataStream & out) const {
    WEvent::serialize(out) << name;
    return out;
}

QDataStream & WSetRemoteName::deserialize(QDataStream & in) {
    WEvent::deserialize(in) >> name;
    return in;
}

