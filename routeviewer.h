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
    void dataFinished();

protected:
    virtual void showEvent(QShowEvent *event);

private:
    Ui::RouteViewer *ui;
    RouteModel *model;

    void setBusyIndicator(bool busy);
};

#endif // ROUTEVIEWER_H
