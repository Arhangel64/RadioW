#include "qsshsocket.h"

QSshSocket::QSshSocket(QObject * parent)
    :QObject(parent),
    loggedIn(false),
    session(ssh_new()),
    m_connected(false),
    executing(false),
    command(0)
{
    int verbosity = SSH_LOG_PROTOCOL;
    ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    qRegisterMetaType<QSshSocket::SshError>(); //not sure if it supposed to be here
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
    
        if (executing) {
            destroyCommand();
        }
        ssh_disconnect(session);
        ssh_free(session);
        session = ssh_new();
        emit disconnected();
    }
}

void QSshSocket::connect(QString host, int port)
{
    if (!m_connected) {
        ssh_options_set(session, SSH_OPTIONS_HOST, host.toLatin1().data());
        ssh_options_set(session, SSH_OPTIONS_PORT, &port);

        int connectionResponse = ssh_connect(session);

        if (connectionResponse == SSH_OK) {
            m_connected = true;
            emit connected();
        } else {
            ssh_disconnect(session);
            ssh_free(session);
            session = ssh_new();
            emit error(SessionCreationError);
        }
    } else {
        throw 1; //TODO
    }
}
void QSshSocket::login(QString user, QString password)
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

void QSshSocket::executeCommand(QString p_command)
{
    if (executing) {
        //todo
        return;
    }
    ssh_channel channel = ssh_channel_new(session);
    if (ssh_channel_open_session(channel) != SSH_OK) {
        emit error(ChannelCreationError);
    }
    int success;
    do {
        success = ssh_channel_request_exec(channel, p_command.toLatin1().data());
    } while (success == SSH_AGAIN);
    
    if (success != SSH_OK) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        emit error(WriteError);
    } else {
        qintptr fd = ssh_get_fd(session);
        QSocketNotifier* readNotifier = new QSocketNotifier(fd, QSocketNotifier::Read);
        QObject::connect(readNotifier, SIGNAL(activated(int)), this, SLOT(socketRead(int)));
    
        command = new Command{fd, p_command, channel, readNotifier};
        executing = true;
    }
}


bool QSshSocket::isConnected()
{
    return m_connected;
}

bool QSshSocket::isLoggedIn()
{
    return loggedIn;
}

bool QSshSocket::isExecuting()
{
    return executing;
}


void QSshSocket::socketRead(int ptr)
{
    command->notifier->setEnabled(false);
    
    QByteArray buffer;
    buffer.resize(1048576);

    int totalBytes = 0;
    int newBytes = 0;
    do {
        newBytes = ssh_channel_read_nonblocking(command->channel, &(buffer.data()[totalBytes]), buffer.size() - totalBytes, 0);
        
        if (newBytes > 0) {
            totalBytes += newBytes;
        }
    
    } while (newBytes > 0);
    
    if (newBytes == SSH_ERROR) {
        emit error(ReadError);
        destroyCommand();
    } else if (ssh_channel_is_eof(command->channel) != 0) {
        command->notifier->setEnabled(true);
        QString response = QString(buffer).mid(0, totalBytes);
        emit commandData(response);
        emit endOfFile();
        destroyCommand();
    } else {
        command->notifier->setEnabled(true);
        QString response = QString(buffer).mid(0, totalBytes);
        emit commandData(response);
    }
}

void QSshSocket::destroyCommand()
{
    delete command->notifier;
    ssh_channel_send_eof(command->channel);
    ssh_channel_close(command->channel);
    ssh_channel_free(command->channel);
    delete command;
    executing = false;
}

