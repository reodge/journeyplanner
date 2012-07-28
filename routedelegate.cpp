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

    QStyledItemDelegate::paint(painter, option, index);

    /* Make sure the painter is left as we found it */
    painter->restore();
}
