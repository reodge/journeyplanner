#include <QDebug>
#include "routemodel.h"

RouteModel::RouteModel(QObject *parent) :
    QStandardItemModel(parent)
{
    /* Add an item for the widget data */
    QStandardItem *item = new QStandardItem(6, 1);
    this->appendRow(item);

    /* Add an item for the route data */
    this->appendRow(new QStandardItem());
}

void RouteModel::getRoutes()
{
}
