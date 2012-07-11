#include "routeviewer.h"
#include "ui_routeviewer.h"

RouteViewer::RouteViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RouteViewer)
{
    ui->setupUi(this);
}

RouteViewer::~RouteViewer()
{
    delete ui;
}
