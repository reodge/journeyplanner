#include "routeviewer.h"
#include "ui_routeviewer.h"
#include <QDebug>
#include <QListWidgetItem>

RouteViewer::RouteViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RouteViewer)
{
    ui->setupUi(this);

    for (int i = 0; i < 20; i++)
    {
        ui->listWidget->addItem(QString::number(i) + "route number");
    }
}

RouteViewer::~RouteViewer()
{
    delete ui;
}

void RouteViewer::itemClicked(QListWidgetItem *item)
{
    qDebug() << item->text();
}
