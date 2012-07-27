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

    setBusyIndicator(true);
    model->getRoutes();
}

void RouteViewer::setModel(RouteModel *model)
{
    /* Remove any connected signals to the previous model */
    disconnect(this, SLOT(dataFinished()));

    this->model = model;
    ui->treeView->setModel(model);

    if (!model)
        return;

    /* Get notified when the model finishes collecting data */
    connect(model, SIGNAL(dataFinished()), this, SLOT(dataFinished()));

    /* Set root view to location of route data in model */
    ui->treeView->setRootIndex(model->item(1)->index());
}

void RouteViewer::dataFinished()
{
    setBusyIndicator(false);
}

void RouteViewer::setBusyIndicator(bool busy)
{
#if defined(Q_WS_MAEMO_5)
    /* Set the twirly busy indicator in the title bar */
    setAttribute(Qt::WA_Maemo5ShowProgressIndicator, busy);
#endif

}
