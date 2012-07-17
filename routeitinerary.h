#ifndef ROUTEITINERARY_H
#define ROUTEITINERARY_H

#include "route.h"
#include <vector>

class RouteItinerary
{
private:
    std::vector<Route> all_routes;

public:
    RouteItinerary();

    int length();
    void addRoute(const Route &r);
    QString routeString(const unsigned n);
};

#endif // ROUTEITINERARY_H
