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

    /* Make single click expand the item */
    connect(ui->treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(handleItemClicked(QModelIndex)));
}

RouteViewer::~RouteViewer()
{
    delete ui;
}

void RouteViewer::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event);

    setBusyIndicator(false);

    if (model)
        model->cancelRoutes();
}

void RouteViewer::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

    setBusyIndicator(true);

    if (model)
        model->getRoutes();
}

void RouteViewer::setModel(RouteModel *model)
{
    /* Remove any connected signals to the previous model */
    disconnect(this, SLOT(handleDataFinished()));

    this->model = model;
    ui->treeView->setModel(model);

    if (!model)
        return;

    /* Get notified when the model finishes collecting data */
    connect(model, SIGNAL(dataFinished()), this, SLOT(handleDataFinished()));

    /* Set root view to location of route data in model */
    ui->treeView->setRootIndex(model->item(1)->index());
}

void RouteViewer::handleDataFinished()
{
    setBusyIndicator(false);
    emit dataFinished();
}

void RouteViewer::setBusyIndicator(bool busy)
{
#if defined(Q_WS_MAEMO_5)
    /* Set the twirly busy indicator in the title bar */
    setAttribute(Qt::WA_Maemo5ShowProgressIndicator, busy);
#endif

}

void RouteViewer::handleItemExpanded(const QModelIndex &index)
{
    int childRows = index.model()->rowCount(index);
    ui->treeView->scrollTo(index.child(childRows - 1, 0));
    ui->treeView->scrollTo(index);
}

void RouteViewer::handleItemClicked(const QModelIndex &index)
{
    if (ui->treeView->isExpanded(index))
        ui->treeView->collapse(index);
    else
        ui->treeView->expand(index);
}
