#include "mainwindow.h"
#include "routemodel.h"

#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    /* Holds our data */
    RouteModel routeModel;

    /* Holds our UI */
    MainWindow mainWindow(&routeModel);

    /* Arrange UI how we want it */
    mainWindow.setOrientation(MainWindow::ScreenOrientationLockLandscape);
    mainWindow.showExpanded();

    /* Add some testing data */
    QStandardItem *parentItem = routeModel.invisibleRootItem();
    for (int i = 0; i < 4; ++i)
    {
        QStandardItem *item = new QStandardItem(QString("item %0").arg(i));
        parentItem->appendRow(item);
        parentItem = item;
    }

    return app.exec();
}
