#include "routedelegate.h"
#include <QDebug>
#include <QPainter>

RouteDelegate::RouteDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void RouteDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    QStyleOptionViewItem newOptions = option;
    if (index.parent().parent() != QModelIndex())
        newOptions.decorationAlignment = Qt::AlignTop;
    else
        newOptions.decorationAlignment = Qt::AlignVCenter;

    QStyledItemDelegate::paint(painter, newOptions, index);

    /* Make sure the painter is left as we found it */
    painter->restore();
}
