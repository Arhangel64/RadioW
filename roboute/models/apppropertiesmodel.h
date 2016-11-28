#ifndef APPPROPERTIESMODEL_H
#define APPPROPERTIESMODEL_H

#include <QtCore/QAbstractTableModel>
#include <QtCore/QMap>
#include <QtCore/QVariant>

#include <deque>
#include <list>

class AppPropertiesModel : public QAbstractTableModel
{
    Q_OBJECT
    
    struct AppProp 
    {
        QString key;
        QString value;
    };
    
public:
    AppPropertiesModel(QObject* parent = 0);
    ~AppPropertiesModel();
    
    void setProp(const QString& key, const QString& value);
    void removeProp(const QString& key);
    void clear();
    
    QVariant data(const QModelIndex &i, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    
private:
    typedef QMap<QString, AppProp*> Map;
    typedef QMap<QString, uint64_t> IndexHelper;
    typedef std::deque<Map::iterator> Index;
    typedef std::pair<QString, AppProp*> Pair;
    typedef std::list<Pair> List;
    
    Index index;
    IndexHelper helper;
    Map map;
    List toInsert;
    
};
    

#endif // APPPROPERTIESMODEL_H
