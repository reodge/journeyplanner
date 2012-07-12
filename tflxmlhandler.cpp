#include "tflxmlhandler.h"
#include <QDebug>

TFLXmlHandler::TFLXmlHandler() :
    QXmlDefaultHandler(),
    new_route(false),
    route_num(1),
    routes(new RouteItinerary)
{
}

bool TFLXmlHandler::startDocument()
{
    qDebug() << ("Parsing started");

    return true;
}

bool TFLXmlHandler::endDocument()
{
    qDebug("Parsing ended");

    return true;
}

bool TFLXmlHandler::startElement(const QString &namespaceURI,
                                 const QString &localName,
                                 const QString &qName,
                                 const QXmlAttributes &atts)
{
    Q_UNUSED (namespaceURI);
    Q_UNUSED (localName);
    Q_UNUSED (atts);

    if (qName == "itdRoute")
    {
        qDebug() << "Route " << route_num;
        new_route = true;
    }

    return true;
}

bool TFLXmlHandler::characters(const QString &ch)
{
    Q_UNUSED (ch);

    return true;
}

bool TFLXmlHandler::endElement(const QString &namespaceURI,
                               const QString &localName,
                               const QString &qName)
{
    Q_UNUSED (namespaceURI);
    Q_UNUSED (localName);

    if (qName == "itdRoute")
        route_num++;

    return true;
}

bool TFLXmlHandler::fatalError(const QXmlParseException &exception)
{
    qDebug() << "Parsing failed: " << exception.message() << endl <<
                " Line: " << exception.lineNumber() << ", Column: " << exception.columnNumber() << endl;

    return false;
}
