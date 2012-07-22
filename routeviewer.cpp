#include "routeviewer.h"
#include "ui_routeviewer.h"
#include <QDebug>
#include <QListWidgetItem>

RouteViewer::RouteViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RouteViewer),
    ri(0)
{
    ui->setupUi(this);
    ui->treeView->header()->hide();
}

RouteViewer::~RouteViewer()
{
    delete ui;
}

void RouteViewer::itemClicked(QListWidgetItem *item)
{
    qDebug() << item->text();
}

void RouteViewer::setModel(QAbstractItemModel *model)
{
    ui->treeView->setModel(model);
}
