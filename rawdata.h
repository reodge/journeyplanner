#ifndef RAWDATA_H
#define RAWDATA_H

#include "position.h"
#include "ui_mainwindow.h"

class RawData
{
private:
    Ui::MainWindow *form;
    Position *pos;

public:
    RawData(Ui::MainWindow *form, Position *pos);

    const QString getOrigin();
};

#endif // RAWDATA_H
