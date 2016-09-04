#include "qsshsocket.h"

QSshSocket::QSshSocket(QObject * parent)
    :QThread(parent),
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
        quit();
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
            start();
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
/*
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
*/

void QSshSocket::executeCommand(QString command)
{
    ssh_channel channel = ssh_channel_new(session);
    if (ssh_channel_open_session(channel) != SSH_OK) {
        emit error(ChannelCreationError);
    }
    qintptr fd = ssh_get_fd(session);
    
    QSocketNotifier* readNotifier = new QSocketNotifier(fd, QSocketNotifier::Read);
    QObject::connect(readNotifier, SIGNAL(activated(int)), this, SLOT(socketRead(int)));
    QSocketNotifier* writeNotifier = new QSocketNotifier(fd, QSocketNotifier::Write);
    QObject::connect(writeNotifier, SIGNAL(activated(int)), this, SLOT(socketWrite(int)));
    
    Command* cmd = new Command{fd, command, channel, false, readNotifier, writeNotifier};
    commands.insert(fd, cmd);
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
    cmd->readNotifier->setEnabled(false);
    
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
        cmd->readNotifier->setEnabled(true);
        QString response = QString(buffer).mid(0, totalBytes);
        emit commandData(cmd->command, response);
    }
}

void QSshSocket::socketWrite(int ptr)
{
    Command* cmd = commands[ptr];
    cmd->writeNotifier->setEnabled(false);
    int nbytes = cmd->command.toLatin1().size();
    int nwritten = ssh_channel_write(cmd->channel, (cmd->command + "\n").toLatin1().data(), nbytes + 1);
    if (nbytes != nwritten) {
        emit error(ScpWriteError);
        destroyCommand(ptr);
    }
    qDebug("written");
    cmd->executed = true;
}

void QSshSocket::destroyCommand(quintptr ptr)
{
    Command* cmd = commands[ptr];
    if (!cmd->executed) {
        ssh_channel_send_eof(cmd->channel);
    }
    ssh_channel_close(cmd->channel);
    ssh_channel_free(cmd->channel);
    delete cmd->readNotifier;
    delete cmd->writeNotifier;
    delete cmd;
    commands.erase(commands.find(ptr));
}

