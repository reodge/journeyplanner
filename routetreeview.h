#ifndef ROUTETREEVIEW_H
#define ROUTETREEVIEW_H

#include <QTreeView>

class RouteTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit RouteTreeView(QWidget *parent = 0);

    void drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const;

private slots:
    bool isTopLevelIndex(const QModelIndex &index) const;
    void handleClicked(const QModelIndex &index);
    void handleExpanded(const QModelIndex &index);
};

#endif // ROUTETREEVIEW_H
