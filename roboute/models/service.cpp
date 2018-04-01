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
    attributes(new C::Attributes(W::Address{u"attributes"})),
    commands(new C::Vocabulary(W::Address{u"management"})),
    login(p_login),
    password(p_password),
    logFile(p_logFile),
    command(p_command),
    psResults(),
    pid(),
    state(Disconnected),
    appState(Unknown),
    name(p_name),
    address(p_address),
    port(p_port),
    id(p_id)
{
    QObject::connect(dataSsh, SIGNAL(opened()), this, SLOT(onDataSshOpened()));
    QObject::connect(dataSsh, SIGNAL(closed()), this, SLOT(onSshClosed()));
    QObject::connect(dataSsh, SIGNAL(data(const QString&)), this, SLOT(onDataSshData(const QString&)));
    QObject::connect(dataSsh, SIGNAL(error(W::SshSocket::Error, const QString&)), this, SLOT(onSshError(W::SshSocket::Error, const QString&)));
    QObject::connect(dataSsh, SIGNAL(finished()), this, SLOT(onDataSshFinished()));
    
    QObject::connect(commandSsh, SIGNAL(opened()), this, SLOT(onCommandSshOpened()));
    QObject::connect(commandSsh, SIGNAL(closed()), this, SLOT(onSshClosed()));
    QObject::connect(commandSsh, SIGNAL(data(const QString&)), this, SLOT(onCommandSshData( const QString&)));
    QObject::connect(commandSsh, SIGNAL(error(W::SshSocket::Error, const QString&)), this, SLOT(onSshError(W::SshSocket::Error, const QString&)));
    QObject::connect(commandSsh, SIGNAL(finished()), this, SLOT(onCommandSshFinished()));
    
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    QObject::connect(socket, SIGNAL(error(W::Socket::SocketError, const QString&)), this, SLOT(onSocketError(W::Socket::SocketError, const QString&)));
    
    QObject::connect(attributes, SIGNAL(attributeChange(const W::String&, const W::Object&)),
                     this, SLOT(onAttrChange(const W::String&, const W::Object&)));
    QObject::connect(attributes, SIGNAL(serviceMessage(const QString&)), SIGNAL(serviceMessage(const QString&)));
    QObject::connect(commands, SIGNAL(serviceMessage(const QString&)), SIGNAL(serviceMessage(const QString&)));
    QObject::connect(commands, SIGNAL(newElement(const W::String&, const W::Object&)), SLOT(onAddCommand(const W::String&, const W::Object&)));
    QObject::connect(commands, SIGNAL(removeElement(const W::String&)), SLOT(onRemoveCommand(const W::String&)));
    QObject::connect(commands, SIGNAL(clear()), SLOT(onClearCommands()));
}

Service::~Service()
{
    delete commands;
    delete attributes;
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

void Service::onDataSshOpened()
{
    if (state == Connecting) {
        state = Echo;
        dataSsh->execute("echo === Roboute connected === >> " + logFile);
        emit serviceMessage("checking log file");
        
    } else {
        //TODO;
    }
}

void Service::onCommandSshOpened()
{
    if (appState == Unknown) {
        appState = Checking;
        requestPid();
        emit serviceMessage("checking if the process launched");
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
                    connectWebsocket();
                    requestPid();
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
    QStringList list = data.split("\n");
    psResults.insert(psResults.end(), list.begin(), list.end());
}

void Service::onCommandSshFinished()
{
    switch (appState) {
        case Checking: 
        case WaitingWebSocket:
        case Active:                //that's very bad!
        {
            bool found = false;
            std::list<QString>::const_iterator itr = psResults.begin();
            std::list<QString>::const_iterator end = psResults.end();
            QString option;
            for (; itr != end; ++itr) {
                option = *itr;
                if (!option.contains(" grep ") && option.contains(command)) {
                    found = true;
                    break;
                }
            }
        
            if (found) {
                QStringList mems = option.split(QRegExp("\\s"));
                QStringList::const_iterator mItr = mems.begin();
                QStringList::const_iterator mEnd = mems.end();
                found = false;
                for (; mItr != mEnd; ++mItr) {
                    QString candidate = *mItr;
                    if (candidate.contains(QRegExp("\\d{2,}"))) {
                        pid = candidate;
                        found = true;
                        break;
                    }
                }
                if (found) {
                    emit serviceMessage("got the process id: " + pid + ", correct?");
                } else {
                    emit serviceMessage("Couldn't find process id");
                }
                
                emit serviceMessage("process seems to be launched");
                
                if (appState == Checking) {
                    connectWebsocket();
                }
            } else {
                appState = Dead;
                emit stopped();
                emit serviceMessage("process seems to be not launched");
            }
            break;
        }
        case Launching:
            emit serviceMessage(QString("process launch command sent,") +
            " requesting pid, waiting for specific 'ready' key in log");        //need to do smthing about this
            break;
        default:
            break;
            
    }
}


void Service::connect()
{
    if (state == Disconnected) {
        dataSsh->open(address);
        commandSsh->open(address);
        state = Connecting;
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
            commands->unsubscribe();
            attributes->unsubscribe();
            socket->close();
        }
        pid = "";
        psResults.clear();
        appState = Unknown;
        emit serviceMessage("disconnecting");
        emit disconnecting();
        emit stopped();
        dataSsh->interrupt();
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
        appState = Launching;
        commandSsh->execute("nohup " + command + " >> " + logFile + " 2>&1 &");
        emit launching();
    }
}

void Service::stop()
{
    if (state == Connected && appState == Active) {
        QString file = command.section("/", -1);
        commandSsh->execute("kill -s SIGINT " + pid);
        appState = Stopping;
        emit stopping();
    }
}


void Service::onSocketConnected()
{
    appState = Active;                  //this is a fail It's not right!
    attributes->subscribe();
    commands->subscribe();
    emit launched();
}

void Service::onSocketDisconnected()
{
    appState = Dead;                    //this is not correct!
    emit stopped();
}

void Service::onSocketError(W::Socket::SocketError err, const QString& msg)
{
    emit serviceMessage(msg);           //this is not correct!
    appState = Dead;
    emit stopped();
}

void Service::registerContollers(W::Dispatcher* dp)
{
    QObject::connect(socket, SIGNAL(message(const W::Event&)), dp, SLOT(pass(const W::Event&)));
    attributes->registerController(dp, socket);
    commands->registerController(dp, socket);
}

void Service::unregisterControllers(W::Dispatcher* dp)
{
    QObject::disconnect(socket, SIGNAL(message(const W::Event&)), dp, SLOT(pass(const W::Event&)));
    commands->unregisterController();
    attributes->unregisterController();
}

void Service::requestPid()
{
    pid = "";
    psResults.clear();
    commandSsh->execute("ps -ax | grep '" + command + "'");
}

void Service::connectWebsocket()
{
    appState = WaitingWebSocket;
    socket->open(W::String(address.toStdString()), W::Uint64(port.toInt()));
    emit serviceMessage("trying to reach service by websocket");
}

void Service::onAttrChange(const W::String& key, const W::Object& value)
{
    emit attributeChanged(QString::fromStdString(key.toString()), QString::fromStdString(value.toString()));
}

void Service::onAddCommand(const W::String& key, const W::Object& value)
{
    QMap<QString, uint64_t> arguments;
    const W::Vocabulary& vc = static_cast<const W::Vocabulary&>(value);
    const W::Vocabulary& args = static_cast<const W::Vocabulary&>(vc.at(u"arguments"));
    
    W::Vector keys = args.keys();
    uint64_t size = keys.length();
    for (int i = 0; i < size; ++i) {
        const W::String& name = static_cast<const W::String&>(keys.at(i));
        const W::Uint64& type = static_cast<const W::Uint64&>(args.at(name));
        
        arguments.insert(QString::fromStdString(name.toString()), type);
    }
    
    emit addCommand(QString::fromStdString(key.toString()), arguments);
}

void Service::onRemoveCommand(const W::String& key)
{
    emit removeCommand(QString::fromStdString(key.toString()));
}

void Service::onClearCommands()
{
    emit clearCommands();
}

void Service::launchCommand(const QString& name, const QMap<QString, QVariant>& args)
{
    const W::Vocabulary& val = static_cast<const W::Vocabulary&>(commands->at(W::String(name.toStdString())));
    const W::Vocabulary& aT = static_cast<const W::Vocabulary&>(val.at(u"arguments"));
    
    QMap<QString, QVariant>::const_iterator itr = args.begin();
    QMap<QString, QVariant>::const_iterator end = args.end();
    W::Vocabulary* vc = new W::Vocabulary();
    
    for (; itr != end; ++itr) {
        W::String wKey(itr.key().toStdString());
        const W::Uint64& wType = static_cast<const W::Uint64&>(aT.at(wKey));
        int type = wType;
        W::Object* value;
        switch (type) {
            case 0:
                value = new W::String(itr.value().toString().toStdString());
                break;
            case 2:
                value = new W::Uint64(itr.value().toInt());
                break;
            default:
                throw 1;
        }
        vc->insert(wKey, value);
    }
    
    W::Event ev(static_cast<const W::Address&>(val.at(u"address")), vc);
    ev.setSenderId(socket->getId());
    socket->send(ev);
}

QMap<QString, QString> Service::getData() const
{
    QMap<QString, QString> data;
    
    data["name"] = name;
    data["address"] = address;
    data["port"] = port;
    data["login"] = login;
    data["password"] = password;
    data["logFile"] = logFile;
    data["command"] = command;
    
    return data;
}

void Service::passNewData(const QMap<QString, QString> data)
{
    if (data.contains("name") && data.value("name") != name) {
        name = data.value("name");
        emit changeName(name);
    }
    
    if (data.contains("address") && data.value("address") != address) {
        address = data.value("address");
    }
    
    if (data.contains("port") && data.value("port") != port) {
        port = data.value("port");
    }
    
    if (data.contains("login") && data.value("login") != login) {
        login = data.value("login");
        dataSsh->setLogin(login);
        commandSsh->setLogin(login);
    }
    
    if (data.contains("password") && data.value("password") != password) {
        password = data.value("password");
        dataSsh->setPassword(password);
        commandSsh->setPassword(password);
    }
    
    if (data.contains("logFile") && data.value("logFile") != logFile) {
        logFile = data.value("logFile");
    }
    
    if (data.contains("command") && data.value("command") != command) {
        command = data.value("command");
    }
}

