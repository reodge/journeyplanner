#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QComboBox>
#include <QtGui/QSlider>
#include <QtGui/QDateTimeEdit>
#include "position.h"

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

public slots:
    void findRoute();

private:
    Ui::MainWindow *ui;
    Position *pos;

    QString comboIndexToUrl(QComboBox *c);
    QString sliderValueToUrl(QSlider *s);
    QString getBaseTFLURL();
    void openTFL();
    void showWaitDialog();
    void showError();

    void setupGeneral();
};

#endif // MAINWINDOW_H
