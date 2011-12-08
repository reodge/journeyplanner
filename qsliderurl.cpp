#include "qsliderurl.h"

QSliderUrl::QSliderUrl(QWidget *parent) :
    QSlider(parent)
{
}

/* Turns current value of slider into string to put into URL */
QString QSliderUrl::toUrlString() const
{
    QString url("&itdTripDateTimeDepArr=");

    switch(this->value())
    {
    case 0:
        return url.append("dep");
    case 1:
        return url.append("arr");
    default:
        return QString("");
    }
}
