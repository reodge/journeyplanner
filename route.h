#ifndef ROUTE_H
#define ROUTE_H

#include <QString>
#include <QTime>

class Route
{
private:
    QTime publicDuration;

public:
    Route();

    void setDuration(const QString &s);

    QTime getDuration();
};

#endif // ROUTE_H
