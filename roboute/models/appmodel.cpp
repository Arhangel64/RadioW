#include "appmodel.h"

AppModel::AppModel(uint64_t p_id, const QString& p_name):
    QObject(),
    id(p_id),
    props(),
    commands(),
    name(p_name),
    log(),
    connectable(false),
    connected(false),
    launchable(false),
    launched(false)
{
}

const QString & AppModel::getName() const
{
    return name;
}

void AppModel::logMessage(const QString& msg)
{
    log.push_back(msg);
    emit newLogMessage(msg);
}

QString* AppModel::getHistory() const
{
    List::const_iterator itr = log.begin();
    List::const_iterator end = log.end();
    
    QString* history = new QString();
    
    for (; itr != end; ++itr) {
        history->append(*itr);
    }
    
    return history;
}

bool AppModel::getConnectable() const
{
    return connectable;
}

bool AppModel::getConnected() const
{
    return connected;
}

bool AppModel::getLaunchable() const
{
    return launchable;
}

bool AppModel::getLaunched() const
{
    return launched;
}

void AppModel::setConnectable(bool value)
{
    if (value != connectable) {
        connectable = value;
        emit changedConnectable(connectable);
    }
}

void AppModel::setConnected(bool value)
{
    if (value != connected) {
        connected = value;
        emit changedConnected(connected);
        emit changedLaunchable(launchable && connected);
    }
}

void AppModel::setLaunchable(bool value)
{
    if (value != launchable) {
        launchable = value;
        emit changedLaunchable(launchable && connected);
    }
}

void AppModel::setLaunched(bool value)
{
    if (value != launched) {
        launched = value;
        emit changedLaunched(launched);
    }
}
