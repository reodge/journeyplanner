#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "position.h"

#include <QtCore/QCoreApplication>
#include <QDesktopServices>
#include "qdatetimeediturl.h"
#include "qsliderurl.h"
#include <QUrl>

#include <cstdio>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    pos(new Position)
{
    ui->setupUi(this);
    setupGeneral();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete pos;
}

void MainWindow::setOrientation(ScreenOrientation orientation)
{
    Qt::WidgetAttribute attribute;
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void MainWindow::showExpanded()
{
#if defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
}

/* Sets up the URL to fetch from tfl and opens it in the default browser */
void MainWindow::openTFL()
{
    QString url("http://journeyplanner.tfl.gov.uk/user/XSLT_TRIP_REQUEST2?language=en&ptOptionsActive=-1&sessionID=0");
    url.append("&name_origin=");
    url.append(ui->lineFrom->text());
    url.append("&type_origin=");
    url.append(ui->comboFrom->toUrlString());
    url.append("&name_destination=");
    url.append(ui->lineTo->text());
    url.append("&type_destination=");
    url.append(ui->comboTo->toUrlString());
    url.append(ui->sldDepArr->toUrlString());
    url.append(ui->dateTime->toUrlString());

    /* For testing */
    printf("Opening URL: %s\n", url.toStdString().c_str());
    fflush(stdout);
    //QDesktopServices::openUrl(QUrl(*url,QUrl::TolerantMode));
}

void MainWindow::setupGeneral()
{
    ui->dateTime->initDateTime();
}
