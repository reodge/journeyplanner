#include "routetreeview.h"
#include <QDebug>

RouteTreeView::RouteTreeView(QWidget *parent) :
    QTreeView(parent)
{
    /* Make single click expand the item */
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(handleClicked(QModelIndex)));
}

void RouteTreeView::handleClicked(const QModelIndex &index)
{
    if (isExpanded(index))
        collapse(index);
    else
        expand(index);
}

void RouteTreeView::handleExpanded(const QModelIndex &index)
{
    int childRows = index.model()->rowCount(index);
    scrollTo(index.child(childRows - 1, 0));
    scrollTo(index);
}
