#ifndef APPLISTMODEL_H
#define APPLISTMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QMap>
#include <vector>
#include <list>

#include "appmodel.h"

class AppListModel : public QAbstractListModel
{
    Q_OBJECT
    
public:
    AppListModel(QObject* parent = 0);
    ~AppListModel();
    
    void push_back(uint64_t id, const QString& name);
    const AppModel* getApp(uint64_t id);
    void clear();
    
    QVariant data(const QModelIndex &i, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    
private:
    typedef QMap<uint64_t, AppModel*> Map;
    typedef std::vector<Map::iterator> Index;
    typedef std::pair<uint64_t, AppModel*> Pair;
    typedef std::list<Pair> List;
    
    Index index;
    Map map;
    List toInsert;
    
public slots:
    void logMessage(uint64_t id, const QString& msg);
};

#endif // APPLISTMODEL_H
