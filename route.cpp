#include "route.h"
#include <QString>
#include <QTime>

Route::Route()
{
}

QString Route::toString() const
{
    QString s("Duration = ");
    s.append(getDuration().toString("hh:mm"));

    return s;
}

void Route::setDuration(const QString &s)
{
    publicDuration = QTime::fromString(s, "hh:mm");
}

QTime Route::getDuration() const
{
    return publicDuration;
}
