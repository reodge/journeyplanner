#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QCoreApplication>
#include <QDebug>
#include "qdatetimeediturl.h"
#include "qsliderurl.h"
#include "position.h"
#include "tflurlgen.h"
#include "routeviewer.h"

#if defined(Q_WS_MAEMO_5)
#include <QtMaemo5>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    rv(new RouteViewer(this)),
    url(this),
    hereRefCount(0)
{
    ui->setupUi(this);
    setupGeneral();
#if defined(Q_WS_MAEMO_5)
    this->setAttribute(Qt::WA_Maemo5StackedWindow);
    rv->setAttribute(Qt::WA_Maemo5StackedWindow);
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
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
    if ((ui->lineFrom->text().isEmpty() && ui->lineFrom->isEnabled()) ||
        (ui->lineTo->text().isEmpty() && ui->lineTo->isEnabled()))
    {
        /* One of the text fields is empty */
        inputDataError();
        return;
    }

    url.openTFL(ui->lineFrom->text(),
                ui->lineTo->text(),
                ui->comboFrom->toUrlString(),
                ui->comboTo->toUrlString(),
                ui->sldDepArr->toUrlString(),
                ui->dateTime->toUrlString());
}

/* Called when an element from combo box "To" is selected */
void MainWindow::indexActivatedTo(const int index)
{
    this->setLineEnabled(this->ui->lineTo, index);
}

/* Called when an element from combo box "From" is selected */
void MainWindow::indexActivatedFrom(const int index)
{
    this->setLineEnabled(this->ui->lineFrom, index);
}

void MainWindow::comboIndexChanged(const int index)
{
    if (index == 4)
    {
        this->hereRefCount++;
        if (this->hereRefCount == 1)
            pos.updatePosition();
    }
    else
    {
        this->hereRefCount--;
        if (this->hereRefCount == 0)
            pos.stopUpdates();
    }
}

void MainWindow::routeDataReady(const RouteItinerary *itinerary)
{
    Q_UNUSED(itinerary);

    qDebug() << "Route data is ready!";

    rv->show();
}

/* Shows an error dialog box. Currently only used if both boxes are empty upon clicking Go */
void MainWindow::inputDataError()
{
    QString errorString("Make sure both text boxes are filled out");

#if defined(Q_WS_MAEMO_5)
    QMaemo5InformationBox::information(NULL, errorString, QMaemo5InformationBox::DefaultTimeout);
#endif

    qDebug() << errorString;
}

/* Helper function to set enabled state of LineEdit From and To widgets based on index from combo boxes */
void MainWindow::setLineEnabled(QLineEdit *l, const int index)
{
    if (index == 4)
    {
        l->setEnabled(false);
    }
    else
    {
        l->setEnabled(true);
        l->setFocus();
    }
}

void MainWindow::setupGeneral()
{
    ui->dateTime->initDateTime();
}
