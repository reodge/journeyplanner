#ifndef ROUTE_H
#define ROUTE_H

#include <QString>
#include <QHash>

class Route
{
private:
    QHash<QString, long long> atts;

public:
    Route();

    void setAttr(const QString &key, const long long &val);

    long long getAttr(const QString &key);
};

#endif // ROUTE_H
