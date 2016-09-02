#include "qsshsocket.h"

QSshSocket::QSshSocket(QString p_user, QObject * parent)
    :QThread(parent),
    loggedIn(false),
    user(p_user),
    session(ssh_new()),
    m_connected(false)
{
    int verbosity = SSH_LOG_PROTOCOL;
    ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
}

QSshSocket::~QSshSocket()
{
    ssh_free(session);
}

void QSshSocket::disconnect()
{
    if (m_connected) {
        loggedIn = false;
        m_connected = false;
        ssh_disconnect(session);
        quit();
    }
}

void QSshSocket::connect(QString host, int port)
{
    if (!m_connected) {
        ssh_options_set(session, SSH_OPTIONS_HOST, host.toLatin1().data());
        ssh_options_set(session, SSH_OPTIONS_USER, user.toLatin1().data());
        ssh_options_set(session, SSH_OPTIONS_PORT, &port);

        int connectionResponse = ssh_connect(session);

        if (connectionResponse == SSH_OK) {
            m_connected = true;
            emit connected();
            start();
        } else {
            emit error(SessionCreationError);
        }
    } else {
        throw 1; //TODO
    }
}
void QSshSocket::login(QString password)
{
    if (m_connected && !loggedIn) {
        int worked = ssh_userauth_password(session, user.toLatin1().data(), password.toLatin1().data());

        if (worked == SSH_OK) {
            loggedIn = true;
            emit loginSuccessful();
        } else {
            emit error(PasswordAuthenticationFailedError);
            disconnect();
        }
    } else {
        throw 2; //TODO
    }
    
}
void QSshSocket::executeCommand(QString command)
{
    ssh_channel channel = ssh_channel_new(session);
    if (ssh_channel_open_session(channel) != SSH_OK) {
        emit error(ChannelCreationError);
    }

    int requestResponse = SSH_AGAIN;

    while (requestResponse == SSH_AGAIN) {
        requestResponse = ssh_channel_request_exec(channel, command.toLatin1().data());
    }

    if (requestResponse != SSH_OK) {
        emit error(ChannelCreationError);
    }

    QByteArray buffer;
    buffer.resize(1000);

    // read in command result
    int totalBytes = 0, newBytes = 0;
    do {
        newBytes = ssh_channel_read(channel, &(buffer.data()[totalBytes]), buffer.size() - totalBytes, 0);
        if (newBytes > 0) {
            totalBytes += newBytes;
        }
    } while (newBytes > 0);

    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);

    QString response = QString(buffer).mid(0, totalBytes);
    emit commandExecuted(response);
}

bool QSshSocket::isConnected()
{
    return m_connected;
}

bool QSshSocket::isLoggedIn()
{
    return loggedIn;
}
