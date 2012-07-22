#include <QtGui>
#include <QDebug>
#include "routeviewer.h"
#include "routemodel.h"
#include "ui_routeviewer.h"

RouteViewer::RouteViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RouteViewer)
{
    ui->setupUi(this);
    ui->treeView->header()->hide();
}

RouteViewer::~RouteViewer()
{
    delete ui;
}

void RouteViewer::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    model->getRoutes();
}

void RouteViewer::setModel(RouteModel *model)
{
    this->model = model;
    ui->treeView->setModel(model);

    if (!model)
        return;

    /* Set root view to location of route data in model */
    ui->treeView->setRootIndex(model->item(1)->index());
}
