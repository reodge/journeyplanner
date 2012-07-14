#include "qsliderurl.h"

QSliderUrl::QSliderUrl(QWidget *parent) :
    QSlider(parent)
{
}

/* Turns current value of slider into string to put into URL */
QString QSliderUrl::toString() const
{
    switch(this->value())
    {
    case 0:
        return QString("dep");
    case 1:
        return QString("arr");
    default:
        return QString("");
    }
}
