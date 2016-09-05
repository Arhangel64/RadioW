#include "service.h"

uint64_t Service::lastId = 0;

Service::Service(
    uint64_t p_id, 
    const QString& p_name, 
    const QString& p_address, 
    const QString& p_port, 
    const QString& p_login, 
    const QString& p_password
):
    QObject(),
    socket(new W::Socket(W::String(u"Roboute"))),
    dataSsh(new W::SshSocket()),
    login(p_login),
    password(p_password),
    name(p_name),
    address(p_address),
    port(p_port),
    id(p_id)
{
    QObject::connect(dataSsh, SIGNAL(opened()), this, SLOT(onSshOpened()));
    QObject::connect(dataSsh, SIGNAL(closed()), this, SLOT(onSshClosed()));
    QObject::connect(dataSsh, SIGNAL(authorized()), this, SLOT(onSshAuthorized()));
    QObject::connect(dataSsh, SIGNAL(data(const QString&, const QString&)), this, SLOT(onSshData(const QString&, const QString&)));
    QObject::connect(dataSsh, SIGNAL(error(W::SshSocket::Error, const QString&)), this, SLOT(onSshError(W::SshSocket::Error, const QString&)));
}

Service::~Service()
{
    delete socket;
    delete dataSsh;
}

Service* Service::create(const QMap<QString, QString>& params)
{
    QString name = params["name"];
    QString address = params["address"];
    QString port = params["port"];
    QString login = params["login"];
    QString password = params["password"];
    
    Service* srv = new Service(++lastId, name, address, port, login, password);
    return srv;
}

void Service::onSshOpened()
{
    emit serviceMessage("socket connected");
    dataSsh->authorize(login, password);
}

void Service::onSshClosed()
{
    emit serviceMessage("socket disconnected");
}

void Service::onSshError(W::SshSocket::Error errCode, const QString& msg)
{
    emit serviceMessage(msg);
}

void Service::onSshAuthorized()
{
    emit serviceMessage("authorized");
    dataSsh->execute("pwd");
}

void Service::onSshData(const QString& command, const QString& data)
{
    emit serviceMessage(data);
}


void Service::connect()
{
    dataSsh->open(address);
}

void Service::disconnect()
{
    dataSsh->close();
}
