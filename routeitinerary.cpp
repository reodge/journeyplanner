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

const Route& RouteItinerary::operator[] (const int i)
{
    return all_routes[i];
}
