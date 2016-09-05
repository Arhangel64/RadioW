#include "qsshsocket.h"

QSshSocket::QSshSocket(QObject * parent)
    :QObject(parent),
    loggedIn(false),
    session(ssh_new()),
    m_connected(false),
    commands()
{
    int verbosity = SSH_LOG_PROTOCOL;
    ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
}

QSshSocket::~QSshSocket()
{
    QMap<qintptr, Command*>::iterator itr = commands.begin();
    QMap<qintptr, Command*>::iterator end = commands.end();
    
    while (itr != end) {
        destroyCommand((*itr)->id);
        itr = commands.begin();
    }
    
    ssh_free(session);
}

void QSshSocket::disconnect()
{
    if (m_connected) {
        loggedIn = false;
        m_connected = false;
        ssh_disconnect(session);
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

void QSshSocket::executeCommand(QString command)
{
    ssh_channel channel = ssh_channel_new(session);
    if (ssh_channel_open_session(channel) != SSH_OK) {
        emit error(ChannelCreationError);
    }
    
    int success = ssh_channel_request_exec(channel, command.toLatin1().data());
    if (success != SSH_OK) {
        ssh_channel_close(channel);
        ssh_channel_free(channel);
        emit error(ScpWriteError);
    } else {
        qintptr fd = ssh_get_fd(session);
        QSocketNotifier* readNotifier = new QSocketNotifier(fd, QSocketNotifier::Read);
        QObject::connect(readNotifier, SIGNAL(activated(int)), this, SLOT(socketRead(int)));
    
        Command* cmd = new Command{fd, command, channel, readNotifier};
        commands.insert(fd, cmd);
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

void QSshSocket::socketRead(int ptr)
{
    Command* cmd = commands[ptr];
    cmd->notifier->setEnabled(false);
    
    QByteArray buffer;
    buffer.resize(256);

    int totalBytes = 0;
    int newBytes = 0;
    do {
        newBytes = ssh_channel_read(cmd->channel, &(buffer.data()[totalBytes]), buffer.size() - totalBytes, 0);
        
        if (newBytes > 0) {
            totalBytes += newBytes;
        }
    } while (newBytes > 0);
    
    if (newBytes < 0) {
        emit error(ScpReadError);
        destroyCommand(ptr);
    } else {
        cmd->notifier->setEnabled(true);
        QString response = QString(buffer).mid(0, totalBytes);
        emit commandData(cmd->command, response);
    }
}

void QSshSocket::destroyCommand(quintptr ptr)
{
    Command* cmd = commands[ptr];
    ssh_channel_send_eof(cmd->channel);
    ssh_channel_close(cmd->channel);
    ssh_channel_free(cmd->channel);
    delete cmd->notifier;
    delete cmd;
    commands.erase(commands.find(ptr));
}

