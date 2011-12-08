#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QCoreApplication>
#include <QDesktopServices>
#include <QDateTime>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupGeneral();
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

void MainWindow::openTFL()
{
    QString *url = new QString("http://journeyplanner.tfl.gov.uk/user/XSLT_TRIP_REQUEST2?language=en&ptOptionsActive=-1&sessionID=0");
    url->append("&name_origin=");
    url->append(ui->lineFrom->text());
    url->append("&type_origin=");
    url->append(comboIndexToUrl(ui->comboFrom));
    url->append("&name_destination=");
    url->append(ui->lineTo->text());
    url->append("&type_destination=");
    url->append(comboIndexToUrl(ui->comboTo));
    QDesktopServices::openUrl(QUrl(*url,QUrl::TolerantMode));
}

/* Sets up elements in the combo box */
void MainWindow::setupCombo(QComboBox *c)
{
    c->addItem("Station or Stop");
    c->addItem("Postcode");
    c->addItem("Address");
    c->addItem("Place of Interest");
}

/* Turns current index from combo box into the string to be added to a url,
 * needs to be kept in sync with MainWindow::setupCombo */
QString MainWindow::comboIndexToUrl(QComboBox *c)
{
    switch(c->currentIndex())
    {
    case 0:
        return QString("stop");
    case 1:
        return QString("locator");
    case 2:
        return QString("address");
    case 3:
        return QString("poi");
    default:
        return QString("");
    }
}

void MainWindow::setupGeneral()
{
    setupCombo(ui->comboFrom);
    setupCombo(ui->comboTo);

    ui->dateTime->setDateTime(QDateTime::currentDateTime());

    connect(ui->btnGo, SIGNAL(released()), this, SLOT(openTFL()));
}
