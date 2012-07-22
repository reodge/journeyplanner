#include "mainwindow.h"
#include "routemodel.h"

#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    /* Holds our data */
    RouteModel routeModel;

    /* Holds our UI */
    MainWindow mainWindow;

    /* Arrange UI how we want it */
    mainWindow.setOrientation(MainWindow::ScreenOrientationLockLandscape);
    mainWindow.showExpanded();

    /* Point UI at our data */
    mainWindow.setModel(&routeModel);

    return app.exec();
}
