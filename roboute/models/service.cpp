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
    dataSsh(new QSshSocket()),
    login(p_login),
    password(p_password),
    name(p_name),
    address(p_address),
    port(p_port),
    id(p_id)
{
    QObject::connect(dataSsh, SIGNAL(connected()), this, SLOT(onDataSshConnected()));
    QObject::connect(dataSsh, SIGNAL(disconnected()), this, SLOT(onDataSshDisconnected()));
    QObject::connect(dataSsh, SIGNAL(loginSuccessful()), this, SLOT(onDataSshLogin()));
    QObject::connect(dataSsh, SIGNAL(error(QSshSocket::SshError)), this, SLOT(onDataSshError(QSshSocket::SshError)));
}

Service::~Service()
{
    delete socket;
    if (dataSsh->isConnected()) {
        dataSsh->disconnect();
    }
    dataSsh->deleteLater();
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

void Service::onDataSshConnected()
{
    emit serviceMessage("socket connected");
    dataSsh->login(login, password);
}

void Service::onDataSshDisconnected()
{
    emit serviceMessage("socket disconnected");
}

void Service::onDataSshError(QSshSocket::SshError err)
{
    switch (err) {
        case QSshSocket::SocketError:
            emit serviceMessage("There was trouble creating a socket. This was most likely due to the lack of an internet connection.");
            break;
        case QSshSocket::SessionCreationError:
            emit serviceMessage("The ssh session could not be created due to inability to find the remote host");
            break;
        case QSshSocket::ChannelCreationError:
            emit serviceMessage("An ssh channel could not be created for the previous operation");
            break;
        case QSshSocket::ScpChannelCreationError:
            emit serviceMessage("An scp channel could not be created for the previous file transfer operation");
            break;
        case QSshSocket::ScpPullRequestError:
            emit serviceMessage("There was an error requesting a pull file transfer");
            break;
        case QSshSocket::ScpPushRequestError:
            emit serviceMessage("There was an error requesting a push file transfer");
            break;
        case QSshSocket::ScpFileNotCreatedError:
            emit serviceMessage("The destination file for the previous transfer does not exist");
            break;
        case QSshSocket::ScpReadError:
            emit serviceMessage("There was an error reading a remote file. This could possibly be due to user permissions");
            break;
        case QSshSocket::ScpWriteError:
            emit serviceMessage("There was an error writing to a remote file. This could possibly be due to user permissions");
            break;
        case QSshSocket::PasswordAuthenticationFailedError:
            emit serviceMessage("The credentials of a user on the remote host could not be authenticated");
            break;
    }
}

void Service::onDataSshLogin()
{
    emit serviceMessage("data logged in");
    dataSsh->executeCommand("pwd");
}

void Service::onDataSshData(QString command, QString data)
{
    emit serviceMessage(data);
}


void Service::connect()
{
    if (!dataSsh->isConnected()) {
        dataSsh->connect(address);
    }
}

void Service::disconnect()
{
    if (dataSsh->isConnected()) {
        dataSsh->disconnect();
    }
}
