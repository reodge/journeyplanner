#include "rawdata.h"
#include "position.h"
#include "ui_mainwindow.h"

RawData::RawData(Ui::MainWindow *form, Position *pos)
{
    this->form = form;
    this->pos = pos;
}

const QString RawData::getOrigin()
{
    return form->lineFrom->text();
}
