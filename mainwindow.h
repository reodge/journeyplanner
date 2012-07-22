#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QStandardItemModel>
#include "position.h"
#include "routedatagen.h"
#include "routeviewer.h"
#include "routeitinerary.h"
#include "rawdata.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    // Note that this will only have an effect on Symbian and Fremantle.
    void setOrientation(ScreenOrientation orientation);

    void showExpanded();

    void setModel(QStandardItemModel *model);

public slots:
    void findRoute();
    void indexActivatedTo(const int index);
    void indexActivatedFrom(const int index);
    void comboIndexChanged(const int index);
    void routeDataReady(RouteItinerary *itinerary);

private:
    Ui::MainWindow *ui;
    RouteViewer rv;

    Position pos;
    RawData data;
    RouteDataGen routeData;
    int hereRefCount;

    QString comboIndexToUrl(QComboBox *c);
    QString sliderValueToUrl(QSlider *s);
    void inputDataError();
    void setLineEnabled(QLineEdit *l, const int index);

    void setupGeneral();
};

#endif // MAINWINDOW_H
