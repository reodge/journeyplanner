#ifndef ROUTEVIEWER_H
#define ROUTEVIEWER_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "routemodel.h"
#include "routeitinerary.h"

namespace Ui {
class RouteViewer;
}

class RouteViewer : public QMainWindow
{
    Q_OBJECT

public slots:
    void itemClicked(QListWidgetItem *item);
    
public:
    explicit RouteViewer(QWidget *parent = 0);
    ~RouteViewer();

    void setModel(RouteModel *model);

protected:
    virtual void showEvent(QShowEvent *event);

private:
    Ui::RouteViewer *ui;
    RouteModel *model;
    RouteItinerary *ri;
};

#endif // ROUTEVIEWER_H
