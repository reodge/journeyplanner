#ifndef TFLXMLHANDLER_H
#define TFLXMLHANDLER_H

#include <QXmlDefaultHandler>
#include <QStandardItemModel>
#include <QDateTime>
#include <QTime>
#include <QStack>

class MeansOfTransport
{
public:
    enum type
    {
        WALK,
        BUS,
        RAIL,
        TRAIN = RAIL,
        TUBE,
        UNKNOWN
    };

    QString name;
    QString from;
    QString to;
    QString endpoint;
    enum type type;

    static enum type decodeType(const QString &category, const QString &type);
};

class TFLXmlHandler;

typedef void (TFLXmlHandler::*StartTagHandlerFn)(const QString &, const QXmlAttributes &);
typedef void (TFLXmlHandler::*EndTagHandlerFn)(const QString &);

class TFLXmlHandler : public QXmlDefaultHandler
{
private:
    QStandardItemModel *model;
    QStandardItem *root;
    QStandardItem *loc; /* seems redundant with root, remove maybe? */
    QString ignoreTag;

    /* Data to pass between tag handlers. */
    QDateTime *currentDateTime;
    QDateTime *routeDepart;
    QDateTime *routeArrive;
    QDateTime *routePartialDepart;
    QDateTime *routePartialArrive;
    QString namePartialDepart;
    QString namePartialArrive;
    QTime routeDuration;
    QTime routePartialDuration;
    QString routePartialName;
    QString routePartialEndpoint;
    QString routePartialType;
    QString routeType;
    QPixmap currentIcon;
    QPixmap routeIcons;

    /* Need to fix up this data problem with all the elements above.
       Start this by se tting up my own data elements to use and fit the parsed data into that,
       rather than following the format given to us. */
    QList<MeansOfTransport> transportList;

    /* Useful helper functions */
    QString resourceFromType(const enum MeansOfTransport::type &type) const;
    QString routeSummary(const enum MeansOfTransport::type &type,
                         const QString &name,
                         const QString &dest,
                         const QString &endPoint) const;
    QPixmap addPixmaps(const QPixmap &p1, const QPixmap &p2);

    /* Handler member function pointers */
    StartTagHandlerFn startTagHandler;
    EndTagHandlerFn endTagHandler;
    QStack<StartTagHandlerFn> startTagHandlerStack;
    QStack<EndTagHandlerFn> endTagHandlerStack;

    /* Useful functions to deal with stack and member function pointers */
    void upOneLevel();
    void downOneLevel(StartTagHandlerFn s, EndTagHandlerFn e);

    /* Worker functions for each xml tag level */
    void itdRequestStart(const QString &name, const QXmlAttributes &atts);
    void itdRequestEnd(const QString &name);
    void itdTripRequestStart(const QString &name, const QXmlAttributes &atts);
    void itdTripRequestEnd(const QString &name);
    void itdItineraryStart(const QString &name, const QXmlAttributes &atts);
    void itdItineraryEnd(const QString &name);
    void itdRouteListStart(const QString &name, const QXmlAttributes &atts);
    void itdRouteListEnd(const QString &name);
    void itdRouteStart(const QString &name, const QXmlAttributes &atts);
    void itdRouteEnd(const QString &name);
    void itdPartialRouteListStart(const QString &name, const QXmlAttributes &atts);
    void itdPartialRouteListEnd(const QString &name);
    void itdPartialRouteStart(const QString &name, const QXmlAttributes &atts);
    void itdPartialRouteEnd(const QString &name);
    void itdPointStart(const QString &name, const QXmlAttributes &atts);
    void itdPointEnd(const QString &name);
    void itdDateTimeStart(const QString &name, const QXmlAttributes &atts);
    void itdDateTimeEnd(const QString &name);
    void itdMeansOfTransportStart(const QString &name, const QXmlAttributes &atts);
    void itdMeansOfTransportEnd(const QString &name);
    void itdOperatorStart(const QString &name, const QXmlAttributes &atts);
    void itdOperatorEnd(const QString &name);
    void itdOperatorNameStart(const QString &name, const QXmlAttributes &atts);
    void itdOperatorNameEnd(const QString &name);
    void itdFrequencyInfoStart(const QString &name, const QXmlAttributes &atts);
    void itdFrequencyInfoEnd(const QString &name);
    void itdFareStart(const QString &name, const QXmlAttributes &atts);
    void itdFareEnd(const QString &name);
    void itdTariffzonesStart(const QString &name, const QXmlAttributes &atts);
    void itdTariffzonesEnd(const QString &name);
    void itdZonesStart(const QString &name, const QXmlAttributes &atts);
    void itdZonesEnd(const QString &name);
    void zoneElemStart(const QString &name, const QXmlAttributes &atts);
    void zoneElemEnd(const QString &name);

public:
    explicit TFLXmlHandler();

    void setModel(QStandardItemModel *model);
    void setRootItem(QStandardItem *item);

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
};

#endif // TFLXMLHANDLER_H
