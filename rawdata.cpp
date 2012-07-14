#include "rawdata.h"
#include "position.h"
#include "ui_mainwindow.h"

RawData::RawData(Ui::MainWindow *form, Position *pos)
{
    this->form = form;
    this->pos = pos;
}

const QString RawData::getOriginType()
{
    return form->comboFrom->toString();
}

const QString RawData::getDestType()
{
    return form->comboTo->toString();
}

const QString RawData::getOrigin()
{
    return form->lineFrom->text();
}

const QString RawData::getDest()
{
    return form->lineTo->text();
}

const QString RawData::getDepArr()
{
    return form->sldDepArr->toString();
}

const QString RawData::getDateTime(const QString& format) const
{
    return form->dateTime->toString(format);
}
