#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QCoreApplication>
#include <QDesktopServices>
#include <QDebug>
#include <QUrl>
#include "qdatetimeediturl.h"
#include "qsliderurl.h"
#include "position.h"

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

/* Checks for data needed and passes off to appropriate function to eventually
   open the TFL website with the inputted data. This indirection currently
   checks for empty from or to boxes and uses the current position if so. */
void MainWindow::findRoute()
{
    if (ui->lineFrom->text().isEmpty())
    {
        if (ui->lineTo->text().isEmpty())
        {
            /* Both boxes are empty, show an error */
            showError();
            return;
        }

        /* Here only the origin box is empty */
    }
    else if (ui->lineTo->text().isEmpty())
    {
        /* Here only the destination box is empty */
    }

    openTFL();
}

/* Returns the main part of the TFL URL */
QString MainWindow::getBaseTFLURL()
{
    QString url("http://journeyplanner.tfl.gov.uk/user/XSLT_TRIP_REQUEST2?language=en&ptOptionsActive=-1&sessionID=0");

    url.append("&type_origin=");
    url.append(ui->comboFrom->toUrlString());
    url.append("&type_destination=");
    url.append(ui->comboTo->toUrlString());
    url.append(ui->sldDepArr->toUrlString());
    url.append(ui->dateTime->toUrlString());

    return url;
}

/* Puts together the data and opens TFL website */
void MainWindow::openTFL()
{
    QString url = getBaseTFLURL();
    url.append("&name_origin=");
    url.append(ui->lineFrom->text());
    url.append("&name_destination=");
    url.append(ui->lineTo->text());

    /* For testing */
    qDebug() << "Opening URL: " << url << endl;
    /* End testing section */

    QDesktopServices::openUrl(QUrl(url,QUrl::TolerantMode));
}

/* Shows a waiting dialog, used while waiting for something long running */
void MainWindow::showWaitDialog()
{
    qDebug() << "Now showing waiting dialog" << endl;
}

/* Shows an error dialog box. Currently only used if both boxes are empty upon clicking Go */
void MainWindow::showError()
{
    qDebug() << "ERROR: Both boxes are empty!" << endl;
}

void MainWindow::setupGeneral()
{
    ui->dateTime->initDateTime();
    pos->updatePosition();
}
