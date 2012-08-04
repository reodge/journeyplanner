#include "tflxmlhandler.h"
#include "routemodel.h"
#include <QDebug>

Q_DECLARE_METATYPE(QXmlAttributes);

#define TAG_FN_EXPAND(FUNC) &TFLXmlHandler::FUNC##Start, &TFLXmlHandler::FUNC##End

TFLXmlHandler::TFLXmlHandler() :
    QXmlDefaultHandler(),
    model(0),
    root(0),
    loc(0)
{
}

void TFLXmlHandler::setDecorations()
{
    QStandardItem *item = model->item(1, 0);

    for (int i = 1; i < item->rowCount() - 1; ++i)
    {
        QStandardItem *route = item->child(i);

        route->setData(getRoutePixmap(route), Qt::DecorationRole);
    }
}

void TFLXmlHandler::setModel(QStandardItemModel *model)
{
    this->model = model;
    root = model->invisibleRootItem();
}

void TFLXmlHandler::setRootItem(QStandardItem *item)
{
    root = item;
}

bool TFLXmlHandler::startDocument()
{
    qDebug() << ("Parsing started");

    if (!model || !root)
    {
        qDebug("Model not set properly");
        return false;
    }

    root->removeRow(root->rowCount() - 1);
    QStandardItem *item = new QStandardItem("Search earlier ...");
    root->appendRow(item);
    loc = root;
    ignoreTag.clear();

    /* Initial handler pointers */
    startTagHandler = &TFLXmlHandler::itdRequestStart;
    endTagHandler = &TFLXmlHandler::itdRequestEnd;

    /* Set up starting state of the tag handler stacks */
    startTagHandlerStack.clear();
    endTagHandlerStack.clear();
    startTagHandlerStack.push(startTagHandler);
    endTagHandlerStack.push(endTagHandler);

    return true;
}

bool TFLXmlHandler::endDocument()
{
    qDebug("Parsing ended");

    QStandardItem *item = new QStandardItem("Search later ...");
    root->appendRow(item);

#if 0
    setDecorations();
#endif

    return true;
}

bool TFLXmlHandler::startElement(const QString &namespaceURI,
                                 const QString &localName,
                                 const QString &qName,
                                 const QXmlAttributes &atts)
{
    Q_UNUSED (namespaceURI);
    Q_UNUSED (localName);

    if (!ignoreTag.isEmpty())
    {
        return true;
    }

    (this->*startTagHandler)(qName, atts);

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

    if (qName == ignoreTag)
    {
        ignoreTag.clear();
        return true;
    }

    (this->*endTagHandler)(qName);

    return true;
}

bool TFLXmlHandler::fatalError(const QXmlParseException &exception)
{
    qDebug() << "Parsing failed: " << exception.message() << endl <<
                " Line: " << exception.lineNumber() << ", Column: " << exception.columnNumber() << endl;

    return false;
}

QString TFLXmlHandler::resourceFromType(const QString type) const
{
    bool ok = false;
    int num = type.toInt(&ok);

    if (!ok)
        return "";

    switch (num)
    {
    case 1:
        return ":/tube";
    case 6:
        return ":/train";
    case 100:
        return ":/walk";
    default:
        return "";
    }
}

QPixmap TFLXmlHandler::getRoutePixmap(const QStandardItem *item) const
{
    QPixmap image;
    QStandardItem *child;
    int i, j;

    for (i = 0; i < item->rowCount(); ++i)
    {
        child = item->child(i);
        if (child->text() == "itdPartialRouteList")
            break;
    }

    if (i == item->rowCount())
    {
        qDebug("Couldn't find itdPartialRouteList");
        return image;
    }

    /* Child is now our itdPartialRouteList, we will add an icon for each partial route we find */
    for (i = 0; i < child->rowCount(); ++i)
    {
        QStandardItem *partialRoute = child->child(i);

        if (partialRoute->text() != "itdPartialRoute")
            continue;

        QStandardItem *mot;
        for (j = 0; j < partialRoute->rowCount(); ++j)
        {
            mot = partialRoute->child(j);
            if (mot->text() == "itdMeansOfTransport")
                break;
        }

        if (j == partialRoute->rowCount())
        {
            qDebug("Couldn't find itdMeansOfTransport");
            continue;
        }

        QXmlAttributes atts = mot->data(RouteModel::AttributesRole).value<QXmlAttributes>();
        QString iconType = atts.value("type");
        QString resourceURL = resourceFromType(iconType);
    }

    return image;
}

void TFLXmlHandler::upOneLevel()
{
    startTagHandler = startTagHandlerStack.pop();
    endTagHandler = endTagHandlerStack.pop();
}

void TFLXmlHandler::downOneLevel(StartTagHandlerFn s, EndTagHandlerFn e)
{
        startTagHandlerStack.push(startTagHandler);
        startTagHandler = s;

        endTagHandlerStack.push(endTagHandler);
        endTagHandler = e;
}

void TFLXmlHandler::itdRequestStart(const QString &name, const QXmlAttributes &atts)
{
    /* Because we don't have a handler for top level, so ignore this */
    if (name == "itdRequest")
        downOneLevel(TAG_FN_EXPAND(itdRequest));
    else if (name == "itdTripRequest")
        downOneLevel(TAG_FN_EXPAND(itdTripRequest));
    else
        ignoreTag = name;
}

void TFLXmlHandler::itdRequestEnd(const QString &name)
{
    if (name == "itdRequest")
        upOneLevel();
}

void TFLXmlHandler::itdTripRequestStart(const QString &name, const QXmlAttributes &atts)
{
    if (name == "itdItinerary")
        downOneLevel(TAG_FN_EXPAND(itdItinerary));
    else
        ignoreTag = name;
}

void TFLXmlHandler::itdTripRequestEnd(const QString &name)
{
    if (name == "itdTripRequest")
        upOneLevel();
}

void TFLXmlHandler::itdItineraryStart(const QString &name, const QXmlAttributes &atts)
{
    if (name == "itdRouteList")
        downOneLevel(TAG_FN_EXPAND(itdRouteList));
}

void TFLXmlHandler::itdItineraryEnd(const QString &name)
{
    if (name == "itdItinerary")
        upOneLevel();
}

void TFLXmlHandler::itdRouteListStart(const QString &name, const QXmlAttributes &atts)
{
    if (name == "itdRoute")
    {
        qDebug("New route");
        routeDuration = QTime::fromString(atts.value("publicDuration"), "hh:mm");
        routeDepart = 0;
        routeArrive = 0;
        downOneLevel(TAG_FN_EXPAND(itdRoute));
    }
}

void TFLXmlHandler::itdRouteListEnd(const QString &name)
{
    if (name == "itdRouteList")
        upOneLevel();
}

void TFLXmlHandler::itdRouteStart(const QString &name, const QXmlAttributes &atts)
{
    if (name == "itdPartialRouteList")
        downOneLevel(TAG_FN_EXPAND(itdPartialRouteList));
    else if (name == "itdFare")
        downOneLevel(TAG_FN_EXPAND(itdFare));
}

void TFLXmlHandler::itdRouteEnd(const QString &name)
{
    if (name == "itdRoute")
    {
        QString summaryString;

        if (routeDepart)
        {
            summaryString += routeDepart->toString("'Dep: 'h:mm' '");
            delete routeDepart;
            routeDepart = 0;
        }

        if (routeArrive)
        {
            summaryString += routeArrive->toString("'Arr: 'h:mm' '");
            delete routeArrive;
            routeArrive = 0;
        }

        summaryString += routeDuration.toString("'Dur: 'hh:mm");

        loc->appendRow(new QStandardItem(summaryString));
        upOneLevel();
    }
}

void TFLXmlHandler::itdPartialRouteListStart(const QString &name, const QXmlAttributes &atts)
{
    if (name == "itdPartialRoute")
        downOneLevel(TAG_FN_EXPAND(itdPartialRoute));
}

void TFLXmlHandler::itdPartialRouteListEnd(const QString &name)
{
    if (name == "itdPartialRouteList")
        upOneLevel();
}

void TFLXmlHandler::itdPartialRouteStart(const QString &name, const QXmlAttributes &atts)
{
    if (name == "itdPoint")
        downOneLevel(TAG_FN_EXPAND(itdPoint));
    else if (name == "itdMeansOfTransport")
        downOneLevel(TAG_FN_EXPAND(itdMeansOfTransport));
}

void TFLXmlHandler::itdPartialRouteEnd(const QString &name)
{
    if (name == "itdPartialRoute")
        upOneLevel();
}

void TFLXmlHandler::itdPointStart(const QString &name, const QXmlAttributes &atts)
{
    if (name == "itdDateTime")
    {
        currentDateTime = new QDateTime();
        downOneLevel(TAG_FN_EXPAND(itdDateTime));
    }
}

void TFLXmlHandler::itdPointEnd(const QString &name)
{
    if (name == "itdPoint")
    {
        if (currentDateTime)
        {
            qDebug() << "Found DateTime:" << currentDateTime->toString();

            if (!routeDepart)
                routeDepart = currentDateTime;
            else
            {
                if (routeArrive)
                    delete routeArrive;

                routeArrive = currentDateTime;
            }

            currentDateTime = 0;
        }
        upOneLevel();
    }
}

void TFLXmlHandler::itdDateTimeStart(const QString &name, const QXmlAttributes &atts)
{
    if (name == "itdDate")
    {
        int y, m, d;
        bool all_ok = true;
        bool this_ok = false;

        y = atts.value("year").toInt(&this_ok);
        all_ok = all_ok && this_ok;

        m = atts.value("month").toInt(&this_ok);
        all_ok = all_ok && this_ok;

        d = atts.value("day").toInt(&this_ok);
        all_ok = all_ok && this_ok;

        if (!all_ok)
        {
            qDebug("Error in decoding itdDate");
            return;
        }

        currentDateTime->setDate(QDate(y, m, d));
    }
    else if (name == "itdTime")
    {
        int h, m;
        bool all_ok = true;
        bool this_ok = false;

        h = atts.value("hour").toInt(&this_ok);
        all_ok = all_ok && this_ok;

        m = atts.value("minute").toInt(&this_ok);
        all_ok = all_ok && this_ok;

        if (!all_ok)
        {
            qDebug("Error in decoding itdTime");
            return;
        }

        currentDateTime->setTime(QTime(h, m, 0));
    }
}

void TFLXmlHandler::itdDateTimeEnd(const QString &name)
{
    if (name == "itdDateTime")
        upOneLevel();
}

void TFLXmlHandler::itdMeansOfTransportStart(const QString &name, const QXmlAttributes &atts)
{
}

void TFLXmlHandler::itdMeansOfTransportEnd(const QString &name)
{
    if (name == "itdMeansOfTransport")
        upOneLevel();
}

void TFLXmlHandler::itdFareStart(const QString &name, const QXmlAttributes &atts)
{
    if (name == "itdTariffzones")
        downOneLevel(TAG_FN_EXPAND(itdTariffzones));
}

void TFLXmlHandler::itdFareEnd(const QString &name)
{
    if (name == "itdFare")
        upOneLevel();
}

void TFLXmlHandler::itdTariffzonesStart(const QString &name, const QXmlAttributes &atts)
{
    if (name == "itdZones")
        downOneLevel(TAG_FN_EXPAND(itdZones));
}

void TFLXmlHandler::itdTariffzonesEnd(const QString &name)
{
    if (name == "itdTariffzones")
        upOneLevel();
}

void TFLXmlHandler::itdZonesStart(const QString &name, const QXmlAttributes &atts)
{
    if (name == "zoneElem")
        downOneLevel(TAG_FN_EXPAND(zoneElem));
}

void TFLXmlHandler::itdZonesEnd(const QString &name)
{
    if (name == "itdZones")
        upOneLevel();
}

void TFLXmlHandler::zoneElemStart(const QString &name, const QXmlAttributes &atts)
{
}

void TFLXmlHandler::zoneElemEnd(const QString &name)
{
    if (name == "zoneElem")
        upOneLevel();
}
