#include "tflxmlhandler.h"
#include "routemodel.h"
#include <QDebug>

Q_DECLARE_METATYPE(QXmlAttributes);

TFLXmlHandler::TFLXmlHandler() :
    QXmlDefaultHandler(),
    model(0),
    root(0),
    loc(0)
{
    initialiseTagAssociations();
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

void TFLXmlHandler::initialiseValidTags()
{
    validTags.clear();
    validTags.append("itdRoute");
}

/* Set up which tags can lead to other tags */
void TFLXmlHandler::initialiseTagAssociations()
{
    tagAssociations.clear();
    tagAssociations.insert("itdRoute", "itdPartialRouteList");
    tagAssociations.insert("itdRoute", "itdFare");
    tagAssociations.insert("itdPartialRouteList", "itdPartialRoute");
    tagAssociations.insert("itdFare", "itdTariffzones");
    tagAssociations.insert("itdPartialRoute", "itdPoint");
    tagAssociations.insert("itdPartialRoute", "itdMeansOfTransport");
    tagAssociations.insert("itdTariffzones", "itdZones");
    tagAssociations.insert("itdPoint", "itdDateTime");
    tagAssociations.insert("itdZones", "zoneElem");
    tagAssociations.insert("itdDateTime", "itdDate");
    tagAssociations.insert("itdDateTime", "itdTime");
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

    initialiseValidTags();
    ignoreTag.clear();
    started = false;

    return true;
}

bool TFLXmlHandler::endDocument()
{
    qDebug("Parsing ended");

    QStandardItem *item = new QStandardItem("Search later ...");
    root->appendRow(item);

    setDecorations();

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

    if (!ignoreTag.isEmpty())
    {
        return true;
    }

    QStandardItem *item = 0;

    if (validTags.contains(qName))
    {
        started = true;
        item = new QStandardItem(qName);
        validTags = tagAssociations.values(qName);
    }
    else if (started)
    {
        ignoreTag = qName;
    }

    if (item)
    {
        item->setData(QVariant::fromValue<QXmlAttributes>(atts), RouteModel::AttributesRole);
        loc->appendRow(item);
        loc = item;
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

    if (qName == ignoreTag)
    {
        ignoreTag.clear();
        return true;
    }

    if (qName == loc->text())
    {
        loc = loc->parent();

        if (loc == root)
            initialiseValidTags();
        else
            validTags = tagAssociations.values(loc->text());
    }

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
