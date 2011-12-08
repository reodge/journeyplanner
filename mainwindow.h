#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QComboBox>
#include <QtGui/QDateTimeEdit>

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
    void openTFL();
    void initDateTime();

private:
    Ui::MainWindow *ui;

    void setupCombo(QComboBox *c);

    QString comboIndexToUrl(QComboBox *c);

    void setupGeneral();
};

#endif // MAINWINDOW_H
