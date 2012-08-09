#include "tflxmlhandler.h"
#include "routemodel.h"
#include <QBrush>
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

QString TFLXmlHandler::resourceFromType(const QString &typeRoute, const QString &typeMOT) const
{
    bool ok = false;
    int num = typeMOT.toInt(&ok);

    if (!ok)
        return "";

    if (typeRoute == "IT")
    {
        switch (num)
        {
        case 100:
            return ":/walk";
        default:
            qDebug() << "resourceFromType unknown IT type:" << num;
            return "";
        }
    }
    else if (typeRoute == "PT")
    {
        switch (num)
        {
        case 1:
            return ":/tube";
        case 3:
            return ":/bus";
        case 6:
            return ":/rail";
        default:
            qDebug() << "resourceFromType unknown PT type:" << num;
            return "";
        }
    }

    qDebug() << "resourceFromType unknown typeRoute:" << typeRoute;
    return "";
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
        routeDepart = 0;
        routeArrive = 0;
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
        routeType = atts.value("type");
        routePartialDepart = 0;
        routePartialArrive = 0;
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
        downOneLevel(TAG_FN_EXPAND(itdPoint));
    else if (name == "itdMeansOfTransport")
    {
        QString s = resourceFromType(routeType, atts.value("type"));
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
        QString times = routePartialDepart->toString("h:mm") + " => " + routePartialArrive->toString("h:mm");
        QStandardItem *item = new QStandardItem(times);
        item->setData(currentIcon, Qt::DecorationRole);
        loc->child(loc->rowCount()-1)->appendRow(item);

        delete routePartialDepart;
        delete routePartialArrive;
        routeType.clear();

        upOneLevel();
    }
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
    {
        if (!routePartialDepart)
            routePartialDepart = new QDateTime(*currentDateTime);
        else if (!routePartialArrive)
            routePartialArrive = new QDateTime(*currentDateTime);

        upOneLevel();
    }
}

void TFLXmlHandler::itdMeansOfTransportStart(const QString &name, const QXmlAttributes &atts)
{
}

void TFLXmlHandler::itdMeansOfTransportEnd(const QString &name)
{
    if (name == "itdMeansOfTransport")
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
