#include "routetreeview.h"
#include <QDebug>

RouteTreeView::RouteTreeView(QWidget *parent) :
    QTreeView(parent)
{
    /* Make single click expand the item */
    connect(this, SIGNAL(clicked(QModelIndex)), this, SLOT(handleClicked(QModelIndex)));
}

void RouteTreeView::mousePressEvent(QMouseEvent *event)
{
    if (DragSelectingState == state())
    {
        /* This seems to fix a bug that required two clicks on an item to open it */
        setState(NoState);
    }
    QTreeView::mousePressEvent(event);
}

void RouteTreeView::drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const
{
    if (isTopLevelIndex(index))
        QTreeView::drawBranches(painter, rect, index);
}

bool RouteTreeView::isTopLevelIndex(const QModelIndex &index) const
{
    return (index.parent() == index.model()->index(1, 0));
}

void RouteTreeView::handleClicked(const QModelIndex &index)
{
    if (isTopLevelIndex(index))
    {
        if (isExpanded(index))
            collapse(index);
        else
            expand(index);
    }
}

void RouteTreeView::handleExpanded(const QModelIndex &index)
{
    int childRows = index.model()->rowCount(index);
    scrollTo(index.child(childRows - 1, 0));
    scrollTo(index);
}
