#ifndef TFLXMLHANDLER_H
#define TFLXMLHANDLER_H

#include "route.h"
#include "routeitinerary.h"
#include <QXmlDefaultHandler>

class TFLXmlHandler : public QXmlDefaultHandler
{
private:
    bool new_route;
    int route_num;
    RouteItinerary *routes;
    Route *current_route;

public:
    explicit TFLXmlHandler();

    bool startDocument();

    bool endDocument();

    bool startElement(const QString &namespaceURI,
                      const QString &localName,
                      const QString &qName,
                      const QXmlAttributes &atts);

    bool characters(const QString &ch);

    bool endElement(const QString &namespaceURI,
                    const QString &localName,
                    const QString &qName);

    bool fatalError(const QXmlParseException &exception);

    bool getRoutes(RouteItinerary *&itinerary);
};

#endif // TFLXMLHANDLER_H
