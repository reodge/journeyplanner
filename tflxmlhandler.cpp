#include "tflxmlhandler.h"
#include "routemodel.h"
#include <QBrush>
#include <QDebug>

Q_DECLARE_METATYPE(QXmlAttributes);

#define TAG_FN_EXPAND(FUNC) &TFLXmlHandler::FUNC##Start, &TFLXmlHandler::FUNC##End

enum MeansOfTransport::type MeansOfTransport::decodeType(const QString &category, const QString &type)
{
    bool ok = false;
    int num = type.toInt(&ok);

    if (!ok)
    {
        qDebug() << "decodeType: type is not an integer:" << type;
        return UNKNOWN;
    }

    if (category == "IT")
    {
        switch (num)
        {
        case 100:
            return WALK;
        default:
            qDebug() << "decodeType: unknown IT type:" << num;
            return UNKNOWN;
        }
    }
    else if (category == "PT")
    {
        switch (num)
        {
        case 1:
            return TUBE;
        case 3:
            return BUS;
        case 6:
            return RAIL;
        default:
            qDebug() << "decodeType: unknown PT type:" << num;
            return UNKNOWN;
        }
    }

    qDebug() << "decodeType: unknown type category:" << category;
    return UNKNOWN;
}

TFLXmlHandler::TFLXmlHandler() :
    QXmlDefaultHandler(),
    model(0),
    root(0),
    loc(0)
{
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

    if (startTagHandler == &TFLXmlHandler::itdOperatorNameStart)
    {
        MeansOfTransport &t = transportList.last();
        t.name += ch;
        qDebug() << "Operator name found: " << ch;
    }

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

QString TFLXmlHandler::resourceFromType(const enum MeansOfTransport::type &type) const
{
    switch (type)
    {
    case MeansOfTransport::WALK:
        return ":/walk";
    case MeansOfTransport::TUBE:
        return ":/tube";
    case MeansOfTransport::BUS:
        return ":/bus";
    case MeansOfTransport::RAIL:
        return ":/rail";
    case MeansOfTransport::UNKNOWN:
        return "";
    default:
        qDebug() << "resourceFromType: unknown enum value:" << type;
        return "";
    }
}

/* This is the same logic as resourceFromType, merge the two? */
QString TFLXmlHandler::routeSummary(const MeansOfTransport &t) const
{
    QString generic = "to " + t.to;

    switch (t.type)
    {
    case MeansOfTransport::WALK:
        return "Walk to " + t.to;
    case MeansOfTransport::TUBE:
        return "Take the " + t.name + " Line towards " + t.endpoint;
    case MeansOfTransport::BUS:
        return "Bus to " + t.to;
    case MeansOfTransport::RAIL:
        if (t.name == "London Overground")
            return "Take the Overground towards " + t.endpoint;
        else
            return "Take the " + t.name + " service towards " + t.endpoint;
    case MeansOfTransport::UNKNOWN:
    default:
        return generic;
    }
}

QPixmap TFLXmlHandler::addPixmaps(const QPixmap &p1, const QPixmap &p2)
{
    QPixmap out;
    if (p1.isNull())
    {
        if (p2.isNull())
            return QPixmap();
        else
            out = QPixmap(p2.size().width(), p2.size().height());
    }
    else
        out = QPixmap(p1.size().width() + p2.size().width(), p1.size().height());

    out.fill();
    QPainter p(&out);
    p.drawPixmap(0, 0, p1);
    p.drawPixmap(p1.size().width(), 0, p2);

    return out;
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
        routeDepart = QDateTime();
        routeArrive = QDateTime();
        routeIcons = QPixmap();
        loc->appendRow(new QStandardItem());
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

        if (routeDepart.isValid())
            summaryString += routeDepart.toString("'Dep: 'h:mm' '");

        if (routeArrive.isValid())
            summaryString += routeArrive.toString("'Arr: 'h:mm' '");

        summaryString += routeDuration.toString("'Dur: 'hh:mm");

        /* Set up the model data */
        QStandardItem *item = loc->child(loc->rowCount()-1);
        item->setData(summaryString, Qt::DisplayRole);
        item->setData(routeIcons, Qt::DecorationRole);

        upOneLevel();
    }
}

void TFLXmlHandler::itdPartialRouteListStart(const QString &name, const QXmlAttributes &atts)
{
    if (name == "itdPartialRoute")
    {
        transportList.clear();
        transportList.append(MeansOfTransport());
        MeansOfTransport &t = transportList.last();

        routePartialType = atts.value("type");
        routePartialDepart = QDateTime();
        routePartialArrive = QDateTime();
        routePartialDuration = QTime::fromString(atts.value("timeMinute"), "m");
        downOneLevel(TAG_FN_EXPAND(itdPartialRoute));
    }
}

void TFLXmlHandler::itdPartialRouteListEnd(const QString &name)
{
    if (name == "itdPartialRouteList")
        upOneLevel();
}

void TFLXmlHandler::itdPartialRouteStart(const QString &name, const QXmlAttributes &atts)
{
    if (name == "itdPoint")
    {
        MeansOfTransport &t = transportList.last();

        if (t.from.isEmpty())
            t.from = atts.value("name");
        else if (t.to.isEmpty())
            t.to = atts.value("name");

        downOneLevel(TAG_FN_EXPAND(itdPoint));
    }
    else if (name == "itdMeansOfTransport")
    {
        MeansOfTransport &t = transportList.last();
        t.type = MeansOfTransport::decodeType(routePartialType, atts.value("type"));
        /* For trains, we'll look at the operator tag */
        if (t.type != MeansOfTransport::RAIL)
        {
            t.name = atts.value("shortname");
        }
        t.endpoint = atts.value("destination");
        QString s = resourceFromType(t.type);
        currentIcon = addPixmaps(QPixmap(), QPixmap(s).scaledToHeight(32, Qt::SmoothTransformation));
        routeIcons = addPixmaps(routeIcons, currentIcon);
        downOneLevel(TAG_FN_EXPAND(itdMeansOfTransport));
    }
    else if (name == "itdFrequencyInfo")
        downOneLevel(TAG_FN_EXPAND(itdFrequencyInfo));
}

void TFLXmlHandler::itdPartialRouteEnd(const QString &name)
{
    if (name == "itdPartialRoute")
    {
        MeansOfTransport &t = transportList.first();
        QString summary = routePartialDepart.toString("h:mm") + " => " + routePartialArrive.toString("h:mm");
        summary += " (" + routePartialDuration.toString("m") + " mins)";
        summary += "\n" + t.from;
        summary += "\n" + routeSummary(t);
        QStandardItem *item = new QStandardItem(summary);
        item->setData(currentIcon, Qt::DecorationRole);
        loc->child(loc->rowCount()-1)->appendRow(item);

        upOneLevel();
    }
}

void TFLXmlHandler::itdPointStart(const QString &name, const QXmlAttributes &atts)
{
    if (name == "itdDateTime")
    {
        currentDateTime = QDateTime();
        downOneLevel(TAG_FN_EXPAND(itdDateTime));
    }
}

void TFLXmlHandler::itdPointEnd(const QString &name)
{
    if (name == "itdPoint")
    {
        if (currentDateTime.isValid())
        {
            if (!routeDepart.isValid())
                routeDepart = currentDateTime;
            else
                routeArrive = currentDateTime;

            currentDateTime = QDateTime();
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

        currentDateTime.setDate(QDate(y, m, d));
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

        currentDateTime.setTime(QTime(h, m, 0));
    }
}

void TFLXmlHandler::itdDateTimeEnd(const QString &name)
{
    if (name == "itdDateTime")
    {
        if (!routePartialDepart.isValid())
            routePartialDepart = currentDateTime;
        else if (!routePartialArrive.isValid())
            routePartialArrive = currentDateTime;

        upOneLevel();
    }
}

void TFLXmlHandler::itdMeansOfTransportStart(const QString &name, const QXmlAttributes &atts)
{
    if (name == "itdOperator")
        downOneLevel(TAG_FN_EXPAND(itdOperator));
}

void TFLXmlHandler::itdMeansOfTransportEnd(const QString &name)
{
    if (name == "itdMeansOfTransport")
        upOneLevel();
}

void TFLXmlHandler::itdOperatorStart(const QString &name, const QXmlAttributes &atts)
{
    if (name == "name")
        downOneLevel(TAG_FN_EXPAND(itdOperatorName));
}

void TFLXmlHandler::itdOperatorEnd(const QString &name)
{
    if (name == "itdOperator")
        upOneLevel();
}

void TFLXmlHandler::itdOperatorNameStart(const QString &name, const QXmlAttributes &atts)
{
}

void TFLXmlHandler::itdOperatorNameEnd(const QString &name)
{
    if (name == "name")
        upOneLevel();
}

void TFLXmlHandler::itdFrequencyInfoStart(const QString &name, const QXmlAttributes &atts)
{
}

void TFLXmlHandler::itdFrequencyInfoEnd(const QString &name)
{
    if (name == "itdFrequencyInfo")
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
