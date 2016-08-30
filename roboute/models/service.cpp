#include "service.h"

uint64_t Service::lastId = 0;

Service::Service(uint64_t p_id, const QString& p_name, const QString& p_address, const QString& p_port):
    socket(new W::Socket(W::String(u"Roboute"))),
    name(p_name),
    address(p_address),
    port(p_port),
    id(p_id)
{
}

Service::~Service()
{
    delete socket;
}

Service* Service::create(const QMap<QString, QString>& params)
{
    QString name = params["name"];
    QString address = params["address"];
    QString port = params["port"];
    
    Service* srv = new Service(++lastId, name, address, port);
    return srv;
}
