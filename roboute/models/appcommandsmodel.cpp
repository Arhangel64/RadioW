#include "appcommandsmodel.h"

AppCommandsModel::AppCommandsModel(QObject* parent):
    QAbstractListModel(parent),
    index(),
    map(),
    toInsert()
{
}

AppCommandsModel::~AppCommandsModel()
{
    clear();
}

void AppCommandsModel::inserCommand(const QString& name, const ArgsMap& args)
{
    toInsert.push_back(Pair(name, new ArgsMap(args)));
    insertRows(rowCount(), 1);
}

void AppCommandsModel::removeCommand(const QString& name)
{
    for (int i = 0; i < index.size(); ++i) {
        if (index[i].key() == name) {
            removeRows(i, 1);
            break;
        }
    }
}

int AppCommandsModel::rowCount(const QModelIndex& parent) const
{
    return index.size();
}

QVariant AppCommandsModel::data(const QModelIndex& i, int role) const
{
    Map::iterator itr = index[i.row()];
    
    switch(role) {
        case Qt::DisplayRole:
            return itr.key();
            
        case Qt::TextAlignmentRole:
            return Qt::AlignCenter + Qt::AlignVCenter;
    }
    
    return QVariant();
}

bool AppCommandsModel::insertRows(int row, int count, const QModelIndex& parent)
{
    if (toInsert.size() != count) {
        return false;
    }
    beginInsertRows(parent, row, row + count - 1);
    
    Index::const_iterator target = index.begin() + row;
    for (int i = 0; i < count; ++i) {
        List::iterator itr = toInsert.begin();
        Map::iterator mItr = map.insert(itr->first, itr->second);
        index.insert(target, mItr);
        toInsert.erase(itr);
    }
    
    endInsertRows();
    return true;
}

bool AppCommandsModel::removeRows(int row, int count, const QModelIndex& parent)
{
    if (row + count > index.size()) {
        return false;
    }
    beginRemoveRows(parent, row, row + count - 1);
    Index::iterator itr;
    Index::iterator beg = index.begin() + row;
    Index::iterator end = beg + count;
    for (itr = beg; itr != end; ++itr) {
        Map::iterator mItr = *itr;
        ArgsMap* app = *mItr;
        delete app;
        map.erase(mItr);
    }
    index.erase(beg, end);
    
    endRemoveRows();
    return true;
}

AppCommandsModel::ArgsMap AppCommandsModel::getCommandArgs(const QString& name)
{
    return *(map[name]);
}

void AppCommandsModel::clear()
{
    beginResetModel();
    Map::iterator itr = map.begin();
    Map::iterator end = map.end();
    
    for (; itr != end; ++itr) {
        delete itr.value();
    }
    map.clear();
    index.clear();
    endResetModel();
}

