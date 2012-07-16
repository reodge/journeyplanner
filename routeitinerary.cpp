#include "routeitinerary.h"

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
