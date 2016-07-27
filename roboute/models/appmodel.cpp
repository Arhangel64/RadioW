#include "appmodel.h"

AppModel::AppModel(uint64_t p_id, const QString& p_name):
    QObject(),
    id(p_id),
    name(p_name),
    log()
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

