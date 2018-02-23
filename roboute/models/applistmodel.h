#ifndef APPLISTMODEL_H
#define APPLISTMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QMap>
#include <QtCore/QVariant>

#include <deque>
#include <list>

#include "appmodel.h"

class AppListModel : public QAbstractListModel
{
    Q_OBJECT
    
public:
    AppListModel(QObject* parent = 0);
    ~AppListModel();
    
    void push_back(uint64_t id, const QString& name);
    void removeElement(uint64_t id);
    AppModel* getApp(uint64_t id);
    void clear();
    
    QVariant data(const QModelIndex &i, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    void setConnectable(uint64_t id, bool value);
    void setConnected(uint64_t id, bool value);
    void setLaunchable(uint64_t id, bool value);
    void setLaunched(uint64_t id, bool value);
    void setEditable(uint64_t id, bool value);
    void setName(uint64_t id, const QString& name);
    void setAttribute(uint64_t id, const QString& key, const QString& value);
    void addCommand(uint64_t id, const QString& key, const QMap<QString, uint64_t>& arguments);
    void removeCommand(uint64_t id, const QString& key);
    void clearCommands(uint64_t id);
    void clearLog(uint64_t id);
    
private:
    typedef QMap<uint64_t, AppModel*> Map;
    typedef QMap<uint64_t, uint64_t> IndexHelper;
    typedef std::deque<Map::iterator> Index;
    typedef std::pair<uint64_t, AppModel*> Pair;
    typedef std::list<Pair> List;
    
    Index index;
    IndexHelper helper;
    Map map;
    List toInsert;
    
public slots:
    void logMessage(uint64_t id, const QString& msg);
};

#endif // APPLISTMODEL_H
