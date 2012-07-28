#ifndef ROUTETREEVIEW_H
#define ROUTETREEVIEW_H

#include <QTreeView>

class RouteTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit RouteTreeView(QWidget *parent = 0);

private slots:
    void handleClicked(const QModelIndex &index);
    void handleExpanded(const QModelIndex &index);
};

#endif // ROUTETREEVIEW_H
