#include "service.h"

uint64_t Service::lastId = 0;

Service::Service(
    uint64_t p_id, 
    const QString& p_name, 
    const QString& p_address, 
    const QString& p_port, 
    const QString& p_login, 
    const QString& p_password,
    const QString& p_logFile,
    const QString& p_command
):
    QObject(),
    socket(new W::Socket(W::String(u"Roboute"))),
    dataSsh(new W::SshSocket()),
    login(p_login),
    password(p_password),
    logFile(p_logFile),
    command(p_command),
    state(Disconnected),
    appState(Dead),
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
    
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    QObject::connect(socket, SIGNAL(error(W::Socket::SocketError, const QString&)), this, SLOT(onSocketError(W::Socket::SocketError, const QString&)));
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
    QString command = params["command"];
    
    Service* srv = new Service(++lastId, name, address, port, login, password, logFile, command);
    return srv;
}

Service* Service::fromSerialized(const QMap<QString, QVariant>& params)
{
    QString name = params["name"].toString();
    QString address = params["address"].toString();
    QString port = params["port"].toString();
    QString login = params["login"].toString();
    QString password = params["password"].toString();
    QString logFile = params["logFile"].toString();
    QString command = params["command"].toString();
    uint64_t id = params["id"].toUInt();
    
    if (id > lastId) {
        lastId = id;
    }
    Service* srv = new Service(id, name, address, port, login, password, logFile, command);
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
    if (appState == Active) {
        socket->close();
    }
    appState = Dead;
    emit serviceMessage("connection clozed");
    emit stopped();
    emit disconnected();
}

void Service::onSshError(W::SshSocket::Error errCode, const QString& msg)
{
    emit serviceMessage(msg);
    switch (state) {
        case Disconnected:
            break;
        case Connecting:
            state = Disconnected;
            emit disconnected();
            break;
        case Authorizing:
        case Echo:
        case Listening:
        case Connected:
            disconnect();
            break;
        default:
            break;
    }
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
                emit serviceMessage("first data from log file, connected!");
            }
        case Connected:
            if (appState == Checking) {
                appState = WaitingWebSocket;
                socket->open(W::String(address.toStdString()), W::Uint64(port.toInt()));
                emit launching();
                emit serviceMessage("Trying to reach service by websocket");
            }
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
        appState = Checking;
        emit serviceMessage("connecting to " + address);
        emit connecting();
    } else {
        //TODO;
    }
}

void Service::disconnect()
{
    if (state != Disconnected) { 
        state = Disconnecting;
        if (appState == Active) {
            socket->close();
        }
        appState = Dead;
        emit serviceMessage("disconnecting");
        emit disconnecting();
        emit stopped();
        dataSsh->close();
    }
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

QVariant Service::saveState() const
{
    QMap<QString, QVariant> state;
    quint64 qid = id;
    state.insert("id", qid);
    state.insert("login", login);
    state.insert("password", password);
    state.insert("logFile", logFile);
    state.insert("name", name);
    state.insert("address", address);
    state.insert("port", port);
    state.insert("command", command);
    
    return state;
}

void Service::launch()
{
    if (state == Connected && appState == Dead) {
        dataSsh->execute("nohup " + command + " >> " + logFile + "&");
        appState = Checking;
        emit launching();
    }
}

void Service::onSocketConnected()
{
    appState = Active;
    emit launched();
}

void Service::onSocketDisconnected()
{
    appState = Dead;
    emit stopped();
}

void Service::onSocketError(W::Socket::SocketError err, const QString& msg)
{
    emit serviceMessage(msg);
    if (appState == WaitingWebSocket) {
        appState = Dead;
        emit stopped();
    }
}

