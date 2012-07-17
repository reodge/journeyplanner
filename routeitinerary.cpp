#include "routeitinerary.h"
#include <assert.h>

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
    s.append(", cTime = ");
    s.append(QString::number(all_routes[n].getAttr("cTime")));

    return s;
}
