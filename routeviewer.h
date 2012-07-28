#ifndef ROUTEVIEWER_H
#define ROUTEVIEWER_H

#include <QtGui>
#include "routemodel.h"

namespace Ui {
class RouteViewer;
}

class RouteViewer : public QMainWindow
{
    Q_OBJECT

public:
    explicit RouteViewer(QWidget *parent = 0);
    ~RouteViewer();

    void setModel(RouteModel *model);

public slots:
    void handleDataFinished();

signals:
    void dataFinished();

protected:
    virtual void hideEvent(QHideEvent *event);
    virtual void showEvent(QShowEvent *event);

private:
    Ui::RouteViewer *ui;
    RouteModel *model;

    void setBusyIndicator(bool busy);

private slots:
    void handleItemClicked(const QModelIndex &index);
    void handleItemExpanded(const QModelIndex &index);
};

#endif // ROUTEVIEWER_H
