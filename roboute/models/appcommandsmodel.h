#ifndef APPCOMMANDSMODEL_H
#define APPCOMMANDSMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QMap>

#include <deque>
#include <list>

class AppCommandsModel : public QAbstractListModel
{
    Q_OBJECT
    
    typedef QMap<QString, uint64_t> ArgsMap;
public:
    AppCommandsModel(QObject* parent = 0);
    ~AppCommandsModel();
    
    void inserCommand(const QString& name, const ArgsMap& args);
    void removeCommand(const QString& name);
    ArgsMap getCommandArgs(const QString& name);
    void clear();
    
    QVariant data(const QModelIndex &i, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    
private:
    typedef QMap<QString, ArgsMap*> Map;
    typedef std::deque<Map::iterator> Index;
    typedef std::pair<QString, ArgsMap*> Pair;
    typedef std::list<Pair> List;
    
    Index index;
    Map map;
    List toInsert;
};

#endif // APPCOMMANDSMODEL_H
