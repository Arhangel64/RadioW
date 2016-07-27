#ifndef APPMODEL_H
#define APPMODEL_H

#include <QtCore/QObject>
#include <list>

class AppModel : public QObject
{
    Q_OBJECT
    
public:
    AppModel(uint64_t p_id, const QString& p_name);
    
    const QString& getName() const;
    void logMessage(const QString& msg);
    QString* getHistory() const;
    
public:
    const uint64_t id;
    
signals:
    void newLogMessage(const QString& msg);
    
private:
    typedef std::list<QString> List;
    QString name;
    List log;
    
};

#endif // APPMODEL_H
