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
        return ":/rail";
    case 100:
        return ":/walk";
    default:
        return "";
    }
}

QPixmap *TFLXmlHandler::addPixmaps(const QPixmap &p1, const QPixmap &p2)
{

    QPixmap *out;
    if (p1.isNull())
    {
        if (p2.isNull())
            return new QPixmap();
        else
            out = new QPixmap(p2.size().width(), p2.size().height());
    }
    else
        out = new QPixmap(p1.size().width() + p2.size().width(), p1.size().height());

    out->fill();
    QPainter p(out);
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
        routeIcons = new QPixmap();
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
        QStandardItem *item = new QStandardItem(summaryString);
        item->setData(*routeIcons, Qt::DecorationRole);
        loc->appendRow(item);

        delete routeIcons;

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
    {
        QString s = resourceFromType(atts.value("type"));
        QPixmap *newIcons = addPixmaps(*routeIcons, QPixmap(s).scaledToHeight(32, Qt::SmoothTransformation));
        delete routeIcons;
        routeIcons = newIcons;
        downOneLevel(TAG_FN_EXPAND(itdMeansOfTransport));
    }
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
