#include "routeitinerary.h"
#include "route.h"
#include <assert.h>
#include <QString>
#include <QTime>

RouteItinerary::RouteItinerary()
{
}

int RouteItinerary::length()
{
    return all_routes.size();
}

void RouteItinerary::addRoute(const Route &r)
{
    all_routes.push_back(r);
}

QString RouteItinerary::routeString(const unsigned n)
{
    if (n > all_routes.size())
        return "";

    QString s("Route ");
    s.append(QString::number(n));
    s.append(", duration = ");
    s.append(all_routes[n].getDuration().toString("hh:mm"));

    return s;
}
