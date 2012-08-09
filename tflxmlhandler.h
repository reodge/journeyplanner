#ifndef TFLXMLHANDLER_H
#define TFLXMLHANDLER_H

#include <QXmlDefaultHandler>
#include <QStandardItemModel>
#include <QDateTime>
#include <QTime>
#include <QStack>

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
    QString routeType;
    QPixmap currentIcon;
    QPixmap routeIcons;

    /* Useful helper functions */
    QString resourceFromType(const QString &typeRoute, const QString &typeMOT) const;
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
