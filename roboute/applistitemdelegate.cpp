#include "applistitemdelegate.h"
#include <iostream>

#define QFIXED_MAX (INT_MAX/256)

AppListItemDelegate::AppListItemDelegate(QObject* parent) :
    QItemDelegate(parent)
{
}


void AppListItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    //Q_ASSERT(index.isValid());

    QStyleOptionViewItem opt = setOptions(index, option);

    // prepare
    painter->save();
//     if (d->clipPainting)
//         painter->setClipRect(opt.rect);

    // get the data and the rectangles

    QVariant value;

    QPixmap pixmap;
    QRect decorationRect;
//     value = index.data(Qt::DecorationRole);
//     if (value.isValid()) {
//         // ### we need the pixmap to call the virtual function
//         pixmap = decoration(opt, value);
//         if (value.type() == QVariant::Icon) {
//             d->tmp.icon = qvariant_cast<QIcon>(value);
//             d->tmp.mode = d->iconMode(option.state);
//             d->tmp.state = d->iconState(option.state);
//             const QSize size = d->tmp.icon.actualSize(option.decorationSize,
//                                                       d->tmp.mode, d->tmp.state);
//             decorationRect = QRect(QPoint(0, 0), size);
//         } else {
//             d->tmp.icon = QIcon();
//             decorationRect = QRect(QPoint(0, 0), pixmap.size());
//         }
//     } else {
//         d->tmp.icon = QIcon();
//         decorationRect = QRect();
//     }

    QString text;
    QRect displayRect;
    value = index.data(Qt::DisplayRole);
    std::cout << "ha" << std::endl;
    if (value.isValid() && !value.isNull()) {
        text = value.toMap().value("name").toString();
        displayRect = opt.rect;
        displayRect.setWidth(QFIXED_MAX);
    }
    QRect checkRect;
    Qt::CheckState checkState = Qt::Unchecked;
//     value = index.data(Qt::CheckStateRole);
//     if (value.isValid()) {
//         checkState = static_cast<Qt::CheckState>(value.toInt());
//         checkRect = check(opt, opt.rect, value);
//     }

    // do the layout

    doLayout(opt, &checkRect, &decorationRect, &displayRect, false);

    // draw the item

    drawBackground(painter, opt, index);
    drawCheck(painter, opt, checkRect, checkState);
    drawDecoration(painter, opt, decorationRect, pixmap);
    drawDisplay(painter, opt, displayRect, text);
    drawFocus(painter, opt, displayRect);

    // done
    painter->restore();
}
