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
    dataSsh(new W::SshSocket(p_login, p_password)),
    commandSsh(new W::SshSocket(p_login, p_password)),
    nodeName(new C::String(W::Address{u"name"})),
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
    QObject::connect(dataSsh, SIGNAL(data(const QString&)), this, SLOT(onDataSshData(const QString&)));
    QObject::connect(dataSsh, SIGNAL(error(W::SshSocket::Error, const QString&)), this, SLOT(onSshError(W::SshSocket::Error, const QString&)));
    QObject::connect(dataSsh, SIGNAL(finished()), this, SLOT(onDataSshFinished()));
    
    QObject::connect(commandSsh, SIGNAL(opened()), this, SLOT(onSshOpened()));
    QObject::connect(commandSsh, SIGNAL(closed()), this, SLOT(onSshClosed()));
    QObject::connect(commandSsh, SIGNAL(data(const QString&)), this, SLOT(onCommandSshData( const QString&)));
    QObject::connect(commandSsh, SIGNAL(error(W::SshSocket::Error, const QString&)), this, SLOT(onSshError(W::SshSocket::Error, const QString&)));
    QObject::connect(commandSsh, SIGNAL(finished()), this, SLOT(onCommandSshFinished()));
    
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    QObject::connect(socket, SIGNAL(error(W::Socket::SocketError, const QString&)), this, SLOT(onSocketError(W::Socket::SocketError, const QString&)));
    
    QObject::connect(nodeName, SIGNAL(change(const QString&)), this, SIGNAL(nodeNameChanged(const QString&)));
}

Service::~Service()
{
    delete nodeName;
    delete commandSsh;
    delete dataSsh;
    delete socket;
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
    if (state == Connecting && dataSsh->isReady() && commandSsh->isReady()) {
        emit serviceMessage("authorized");
        state = Echo;
        dataSsh->execute("echo === Roboute connected === >> " + logFile);
        emit serviceMessage("checking log file");
        
        if (appState == Checking) {
            appState = WaitingWebSocket;
            socket->open(W::String(address.toStdString()), W::Uint64(port.toInt()));
            emit launching();
            emit serviceMessage("Trying to reach service by websocket");
        }
    } else {
        //TODO;
    }
}

void Service::onSshClosed()
{
    if (state == Disconnected) {
        emit serviceMessage("connection clozed");
        emit stopped();
        emit disconnected();
    }
    if (state == Disconnecting) {
        state = Disconnected;
    }
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
        case Echo:
        case Listening:
        case Connected:
            disconnect();
            break;
        default:
            break;
    }
}

void Service::onDataSshData(const QString& data)
{
    switch (state) {
        case Listening:
            state = Connected;
            emit connected();
            emit serviceMessage("first data from log file, connected!");
        case Connected:
            if (appState == Launching) {
                if (data.contains("ready")) {
                    appState = WaitingWebSocket;
                    socket->open(W::String(address.toStdString()), W::Uint64(port.toInt()));
                }
            }
            emit log(data);
            break;
        default:
            break;
    }
}

void Service::onCommandSshData(const QString& data)
{
    
}

void Service::onCommandSshFinished()
{
    if (appState == Checking) {
        appState = Launching;
        emit launching();
        emit serviceMessage("Trying to reach service by websocket");
    }
}


void Service::connect()
{
    if (state == Disconnected) {
        dataSsh->open(address);
        commandSsh->open(address);
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
            nodeName->unsubscribe();
            socket->close();
        }
        appState = Dead;
        emit serviceMessage("disconnecting");
        emit disconnecting();
        emit stopped();
        dataSsh->close();
        commandSsh->close();
    }
}

void Service::onDataSshFinished()
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
        appState = Checking;
        commandSsh->execute("nohup " + command + " >> " + logFile + "&");
        emit launching();
    }
}

void Service::stop()
{
    if (state == Connected && appState == Active) {
        QString file = command.section("/", -1);
        commandSsh->execute("killall -SIGINT " + file);
        appState = Stopping;
        emit stopping();
    }
}


void Service::onSocketConnected()
{
    appState = Active;
    nodeName->subscribe();
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
    appState = Dead;
    emit stopped();
}

void Service::registerContollers(W::Dispatcher* dp)
{
    QObject::connect(socket, SIGNAL(message(const W::Event&)), dp, SLOT(pass(const W::Event&)));
    nodeName->registerController(dp, socket);
}

void Service::unregisterControllers(W::Dispatcher* dp)
{
    QObject::disconnect(socket, SIGNAL(message(const W::Event&)), dp, SLOT(pass(const W::Event&)));
    nodeName->unregisterController();
}






