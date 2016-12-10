#include "sshsocket.h"
#include <QtCore/QDebug>

W::SshSocket::SshSocket(const QString& p_login, const QString& p_password, QObject* parent):
    QObject(parent),
    socket(new QSshSocket()),
    thread(new QThread()),
    login(p_login),
    password(p_password),
    state(Disconnected)
{
    connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    connect(socket, SIGNAL(loginSuccessful()), this, SLOT(onSocketLoggedIn()));
    connect(socket, SIGNAL(error(QSshSocket::SshError)), this, SLOT(onSocketError(QSshSocket::SshError)));
    connect(socket, SIGNAL(commandData(QString)), this, SLOT(onSocketCommandData(QString)));
    connect(socket, SIGNAL(endOfFile()), this, SLOT(onSocketEOF()));
    
    socket->moveToThread(thread);
}

W::SshSocket::~SshSocket()
{
    if (state != Disconnected) {
        if (state == Disconnecting) {
            onSocketDisconnected();
        } else {
            qDebug("Socket wasn't closed, terminating the inner thread");
            thread->terminate();
        }
    }
    socket->deleteLater();
    thread->deleteLater();
    //TODO;
}

void W::SshSocket::open(const QString& address, uint16_t port)
{
    if (state == Disconnected) {
        state = Connecting;
        thread->start();
        QMetaObject::invokeMethod(socket, "connect", Qt::QueuedConnection, Q_ARG(QString, address), Q_ARG(int, port));
    } else {
        //TODO;
    }
}

void W::SshSocket::onSocketConnected()
{
    if (state == Connecting) {
        state = Connected;
        authorize();
    } else {
        //TODO;
    }
}

void W::SshSocket::authorize()
{
    if (state == Connected) {
        state = Authorizing;
        QMetaObject::invokeMethod(socket, "login", Qt::QueuedConnection, Q_ARG(QString, login), Q_ARG(QString, password));
    } else {
        //TODO;
    }
}

void W::SshSocket::onSocketLoggedIn()
{
    if (state == Authorizing) {
        state = Authorized;
        emit opened();
    }
}

void W::SshSocket::close()
{
    switch (state) {
        case Disconnected:
            //TODO;
            break;
        case Connecting:
        case Connected:
        case Authorizing:
        case Authorized:
            QMetaObject::invokeMethod(socket, "disconnect", Qt::QueuedConnection);
            state = Disconnecting;
            break;
        case Disconnecting:
            //TODO;
            break;
    }
}

void W::SshSocket::onSocketDisconnected()
{
    if (state == Disconnecting) {
        thread->quit();
        thread->wait();
        state = Disconnected;
        emit closed();
    } else {
        //TODO;
    }
}

void W::SshSocket::execute(const QString& command)
{
    if (state == Authorized) {
        QMetaObject::invokeMethod(socket, "executeCommand", Qt::QueuedConnection, Q_ARG(QString, command));
    } else {
        //TODO;
    }
}

void W::SshSocket::onSocketCommandData(QString p_data)
{
    if (state == Authorized) {
        emit data(p_data);
    }
}

void W::SshSocket::onSocketError(QSshSocket::SshError p_error)
{
    QString msg;
    Error errCode;
    switch (p_error) {
        case QSshSocket::SocketError:
            msg = "There was a trouble creating a socket. Looks like you have problems with internet connectiion";
            errCode = SocketError;
            break;
        case QSshSocket::SessionCreationError:
            msg = "No route to the remote host";
            errCode = SessionCreationError;
            if (state == Connecting) {
                state = Disconnected;
            }
            break;
        case QSshSocket::ChannelCreationError:
            msg = "An ssh channel could not be created";
            errCode = ChannelCreationError;
            break;
        case QSshSocket::ReadError:
            msg = "There was an error reading the socket";
            errCode = ReadError;
            break;
        case QSshSocket::WriteError:
            msg = "There was an error writing to the socket";
            errCode = WriteError;
            break;
        case QSshSocket::PasswordAuthenticationFailedError:
            msg = "The credentials of a user on the remote host could not be authenticated";
            errCode = PasswordAuthenticationError;
            if (state == Authorizing) {
                state = Connected;
            }
            break;
    }
    
    emit error(errCode, msg);
}

void W::SshSocket::onSocketEOF()
{
    emit finished();
}

bool W::SshSocket::isReady() const
{
    return state == Authorized;
}







