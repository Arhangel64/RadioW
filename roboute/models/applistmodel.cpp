#include "applistmodel.h"
#include <iostream>

AppListModel::AppListModel(QObject* parent):
    QAbstractListModel(parent),
    index(),
    helper(),
    map(),
    toInsert()
{
    
}

AppListModel::~AppListModel()
{
    clear();
}


void AppListModel::push_back(uint64_t id, const QString& name)
{
    AppModel* item = new AppModel(id, name);
    
    toInsert.push_back(Pair(id, item));
    insertRows(rowCount(), 1);
}

void AppListModel::removeElement(uint64_t id)
{
    int index = *(helper.find(id));
    removeRows(index, 1);
}


int AppListModel::rowCount(const QModelIndex& parent) const
{
    //std::cout << index.size() << std::endl;
    return index.size();
}

QVariant AppListModel::data(const QModelIndex& i, int role) const
{
    Map::iterator itr = index[i.row()];
    
    switch(role) {
        case Qt::DisplayRole:
            return itr.value()->getName();
            
        case Qt::TextAlignmentRole:
            return Qt::AlignCenter + Qt::AlignVCenter;
            
        case Qt::UserRole:
            quint64 id = itr.key();
            return id;
    }
    
    return QVariant();
}

bool AppListModel::insertRows(int row, int count, const QModelIndex& parent)
{
    if (toInsert.size() != count) {
        return false;
    }
    beginInsertRows(parent, row, count);
    
    Index::const_iterator target = index.begin() + row;
    for (int i = 0; i < count; ++i) {
        List::iterator itr = toInsert.begin();
        Map::iterator mItr = map.insert(itr->first, itr->second);
        index.insert(target, mItr);
        helper.insert(itr->first, row + i);
        toInsert.erase(itr);
    }
    
    endInsertRows();
    return true;
}

bool AppListModel::removeRows(int row, int count, const QModelIndex& parent)
{
    if (row + count > index.size()) {
        return false;
    }
    beginRemoveRows(parent, row, count);
    Index::iterator itr;
    Index::iterator beg = index.begin() + row;
    Index::iterator end = beg + count;
    for (itr = beg; itr != end; ++itr) {
        Map::iterator mItr = *itr;
        AppModel* app = *mItr;
        IndexHelper::iterator hItr = helper.find(app->id);
        delete app;
        map.erase(mItr);
        helper.erase(hItr);
    }
    index.erase(beg, end);
    
    endRemoveRows();
    return true;
}

void AppListModel::logMessage(uint64_t id, const QString& msg)
{
    map[id]->logMessage(msg);
    
}

AppModel* AppListModel::getApp(uint64_t id)
{
    return map[id];
}

void AppListModel::clear()
{
    beginResetModel();
    Map::iterator itr = map.begin();
    Map::iterator end = map.end();
    
    for (; itr != end; ++itr) {
        delete itr.value();
    }
    map.clear();
    index.clear();
    helper.clear();
    endResetModel();
}

void AppListModel::setConnectable(uint64_t id, bool value)
{
    map[id]->setConnectable(value);
}

void AppListModel::setConnected(uint64_t id, bool value)
{
    map[id]->setConnected(value);
}

void AppListModel::setLaunchable(uint64_t id, bool value)
{
    map[id]->setLaunchable(value);
}

void AppListModel::setLaunched(uint64_t id, bool value)
{
    map[id]->setLaunched(value);
}

void AppListModel::setProp(uint64_t id, const QString& key, const QString& value)
{
    map[id]->props.setProp(key, value);
}
