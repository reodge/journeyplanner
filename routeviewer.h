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

protected:
    virtual void showEvent(QShowEvent *event);

private:
    Ui::RouteViewer *ui;
    RouteModel *model;
};

#endif // ROUTEVIEWER_H
