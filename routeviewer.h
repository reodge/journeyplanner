#ifndef ROUTEVIEWER_H
#define ROUTEVIEWER_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QStandardItemModel>
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

    void setModel(QStandardItemModel *model);
    
private:
    Ui::RouteViewer *ui;
    RouteItinerary *ri;
};

#endif // ROUTEVIEWER_H
