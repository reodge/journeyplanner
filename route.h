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

    QString toString() const;

    void setDuration(const QString &s);

    QTime getDuration() const;
};

#endif // ROUTE_H
