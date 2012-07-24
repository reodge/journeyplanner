#include <QtGui>
#include <QDebug>
#include "routemodel.h"

RouteModel::RouteModel(QObject *parent) :
    QStandardItemModel(parent),
    data(this)
{
    /* Add an item for the widget data */
    QStandardItem *item = new QStandardItem(LAYOUT_MAX, 1);
    this->appendRow(item);

    /* Add an item for the route data */
    item = new QStandardItem();
    this->appendRow(item);
}

void RouteModel::findPositionHint(bool state)
{
    if (state)
        pos.updatePosition();
    else
        pos.stopUpdates();
}

void RouteModel::getRoutes()
{
    data.setModel(this);
    data.setRootIndex(this->item(1)->index());
    data.getData();
}
