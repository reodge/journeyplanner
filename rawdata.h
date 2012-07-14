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

    const QString getOriginType();
    const QString getDestType();

    const QString getOrigin();
    const QString getDest();

    const QString getDepArr();
    const QString getDateTime(const QString& format) const;
};

#endif // RAWDATA_H
