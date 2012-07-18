#include "route.h"
#include <QString>
#include <QTime>

Route::Route()
{
}

void Route::setDuration(const QString &s)
{
    publicDuration = QTime::fromString(s, "hh:mm");
}

QTime Route::getDuration()
{
    return publicDuration;
}
