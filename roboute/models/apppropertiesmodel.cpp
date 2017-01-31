#include "apppropertiesmodel.h"

AppPropertiesModel::AppPropertiesModel(QObject* parent):
    QAbstractTableModel(parent),
    index(),
    helper(),
    map(),
    toInsert()
{
}

AppPropertiesModel::~AppPropertiesModel()
{
    clear();
}


void AppPropertiesModel::clear()
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

int AppPropertiesModel::columnCount(const QModelIndex& parent) const
{
    return 2;
}

bool AppPropertiesModel::insertRows(int row, int count, const QModelIndex& parent)
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
        helper.insert(itr->first, row + i);
        toInsert.erase(itr);
    }
    
    endInsertRows();
    return true;
}

bool AppPropertiesModel::removeRows(int row, int count, const QModelIndex& parent)
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
        AppProp* prop = *mItr;
        IndexHelper::iterator hItr = helper.find(prop->key);
        delete prop;
        map.erase(mItr);
        helper.erase(hItr);
    }
    index.erase(beg, end);
    
    endRemoveRows();
    return true;
}

int AppPropertiesModel::rowCount(const QModelIndex& parent) const
{
    return index.size();
}

void AppPropertiesModel::setProp(const QString& key, const QString& value)
{
    Map::iterator itr = map.find(key);
    if (itr != map.end()) {
        itr.value()->value = value;
        const QModelIndex ind = QAbstractTableModel::index(*(helper.find(key)), 1);
        emit dataChanged(ind, ind);
    } else {
        AppProp* item = new AppProp{key, value};
    
        toInsert.push_back(Pair(key, item));
        insertRows(rowCount(), 1);
    }
}

void AppPropertiesModel::removeProp(const QString& key)
{
    IndexHelper::iterator itr = helper.find(key);
    if (itr != helper.end()) {
        removeRows(*itr, 1);
    }
}

QVariant AppPropertiesModel::data(const QModelIndex& i, int role) const
{
    Map::iterator itr = index[i.row()];
    int col = i.column();
    
    switch(role) {
        case Qt::DisplayRole:
            if (col == 0) {
                return itr.key();
            } else if (col == 1) {
                return itr.value()->value;
            }
    }
    
    return QVariant();
}

QVariant AppPropertiesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (orientation) {
        case Qt::Horizontal:
            switch(role) {
                case Qt::DisplayRole:
                    if (section == 0) {
                        return "Key";
                    } else if (section == 1) {
                        return "Value";
                    }
            }
    }
    
    
    return QVariant();
}

