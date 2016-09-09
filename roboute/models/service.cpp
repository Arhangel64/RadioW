#include "service.h"

uint64_t Service::lastId = 0;

Service::Service(
    uint64_t p_id, 
    const QString& p_name, 
    const QString& p_address, 
    const QString& p_port, 
    const QString& p_login, 
    const QString& p_password,
    const QString& p_logFile
):
    QObject(),
    socket(new W::Socket(W::String(u"Roboute"))),
    dataSsh(new W::SshSocket()),
    login(p_login),
    password(p_password),
    logFile(p_logFile),
    state(Disconnected),
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
    QObject::connect(dataSsh, SIGNAL(finished(const QString&)), this, SLOT(onSshFinished(const QString&)));
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
    QString logFile = params["logFile"];
    
    Service* srv = new Service(++lastId, name, address, port, login, password, logFile);
    return srv;
}

void Service::onSshOpened()
{
    if (state == Connecting) {
        emit serviceMessage("connection established");
        state = Authorizing;
        dataSsh->authorize(login, password);
        emit serviceMessage("authorizing");
    } else {
        //TODO;
    }
}

void Service::onSshClosed()
{
    if (state == Disconnecting) {
        state = Disconnected;
    }
    emit serviceMessage("connection clozed");
    emit disconnected();
}

void Service::onSshError(W::SshSocket::Error errCode, const QString& msg)
{
    emit serviceMessage(msg);
}

void Service::onSshAuthorized()
{
    if (state == Authorizing) {
        emit serviceMessage("authorized");
        state = Echo;
        dataSsh->execute("echo === Roboute connected === >> " + logFile);
        emit serviceMessage("checking log file");
    } else {
        //TODO;
    }
}

void Service::onSshData(const QString& command, const QString& data)
{
    QString tail = "tail -f " + logFile;
    switch (state) {
        case Listening:
            if (command == tail) {
                state = Connected;
                emit connected();
                emit serviceMessage("first data from lig file, connected!");
            }
        case Connected:
            if (command == tail) {
                emit log(data);
            }
            break;
        default:
            break;
    }
}


void Service::connect()
{
    if (state == Disconnected) {
        dataSsh->open(address);
        state = Connecting;
        emit serviceMessage("connecting to " + address);
        emit connecting();
    } else {
        //TODO;
    }
}

void Service::disconnect()
{
    state = Disconnecting;
    emit serviceMessage("disconnecting");
    emit disconnecting();
    dataSsh->close();
}

void Service::onSshFinished(const QString& command)
{
    switch (state) {
        case Echo:
            emit serviceMessage("log file checked");
            dataSsh->execute("tail -f " + logFile);
            state = Listening;
            emit serviceMessage("listening to the log file");
            break;
        default:
            break;
    }
}



