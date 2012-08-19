#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QCoreApplication>
#include <QtGui>
#include <QModelIndex>
#include <QDebug>
#include "qdatetimeediturl.h"
#include "qsliderurl.h"
#include "routeviewer.h"
#include "routemodel.h"

#if defined(Q_WS_MAEMO_5)
#include <QtMaemo5>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    rv(this),
    mapper(this),
	model(0),
    posRefCount(2) /* It will be decremented twice on initialisation of UI */
{
    /* Initialise UI elements */
    ui->setupUi(this);
    setupGeneral();

    /* Set some global config for widget mapper */
    mapper.setOrientation(Qt::Vertical);
    mapper.setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    /* Ensure our windows have the cool back arrow on Maemo */
#if defined(Q_WS_MAEMO_5)
    this->setAttribute(Qt::WA_Maemo5StackedWindow);
    rv.setAttribute(Qt::WA_Maemo5StackedWindow);
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

void MainWindow::setModel(RouteModel *model)
{
    /* Save it locally */
    this->model = model;

    if (!model)
    {
        mapper.setModel(0);
        return;
    }

    /* Configure widget mapper */
    mapper.setModel(model);

    /* Get model element that points to raw data and use it for mapper */
    QModelIndex index = model->indexFromItem(model->item(0));
    mapper.setRootIndex(index);

    /* Set widgets to use this model */
    mapper.addMapping(ui->comboFrom, RouteModel::LAYOUT_FROM_TYPE, "currentIndex");
    mapper.addMapping(ui->lineFrom, RouteModel::LAYOUT_FROM_NAME, "text");
    mapper.addMapping(ui->comboTo, RouteModel::LAYOUT_TO_TYPE, "currentIndex");
    mapper.addMapping(ui->lineTo, RouteModel::LAYOUT_TO_NAME, "text");
    mapper.addMapping(ui->sldDepArr, RouteModel::LAYOUT_DEPARR, "value");
    mapper.addMapping(ui->dateTime, RouteModel::LAYOUT_DATETIME, "dateTime");
    mapper.toFirst();

    /* Ensure initial data is set up in the model */
    mapper.submit();

    rv.setModel(model);
}

/* Checks for data needed and passes off to appropriate function to eventually
   open the TFL website with the inputted data. This indirection currently
   checks for empty from or to boxes and uses the current position if so. */
void MainWindow::findRoute()
{
    /* Validate form */
    if ((ui->lineFrom->text().isEmpty() && ui->lineFrom->isEnabled()) ||
        (ui->lineTo->text().isEmpty() && ui->lineTo->isEnabled()))
    {
        /* One of the text fields is empty */
        inputDataError();
        return;
    }

    rv.show();
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
        posRefCount++;
        if ((posRefCount == 1) && model)
            model->findPositionHint(true);
    }
    else
    {
        posRefCount--;
        if ((posRefCount == 0) && model)
            model->findPositionHint(false);
    }

    /* This should work with AutoSubmit, but doesn't for ComboBox, so we do it here */
    mapper.submit();
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
