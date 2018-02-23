#ifndef APPMODEL_H
#define APPMODEL_H

#include "apppropertiesmodel.h"
#include "appcommandsmodel.h"

#include <QtCore/QObject>
#include <list>

class AppModel : public QObject
{
    Q_OBJECT
    
public:
    AppModel(uint64_t p_id, const QString& p_name);
    
    const QString& getName() const;
    void setName(const QString& p_name);
    void logMessage(const QString& msg);
    QString* getHistory() const;
    bool getConnectable() const;
    bool getConnected() const;
    bool getLaunchable() const;
    bool getLaunched() const;
    bool getEditable() const;
    void clearLog();
    
public:
    const uint64_t id;
    AppPropertiesModel props;
    AppCommandsModel commands;
    
signals:
    void newLogMessage(const QString& msg);
    void changedConnectable(bool value);
    void changedConnected(bool value);
    void changedLaunchable(bool value);
    void changedLaunched(bool value);
    void changedEditable(bool value);
    void clearedLog();
    
public slots:
    void setConnectable(bool value);
    void setConnected(bool value);
    void setLaunchable(bool value);
    void setLaunched(bool value);
    void setEditable(bool value);
    
private:
    typedef std::list<QString> List;
    QString name;
    List log;
    bool connectable;
    bool connected;
    bool launchable;
    bool launched;
    bool editable;
    
};

#endif // APPMODEL_H
