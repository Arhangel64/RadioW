#ifndef APPLISTITEMDELEGATE_H
#define APPLISTITEMDELEGATE_H

#include <QtWidgets/QItemDelegate>
#include <QtGui/QPainter>

class AppListItemDelegate : public QItemDelegate
{
    Q_OBJECT
    
public:
    AppListItemDelegate(QObject* parent = 0);
    
    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

#endif // APPLISTITEMDELEGATE_H
