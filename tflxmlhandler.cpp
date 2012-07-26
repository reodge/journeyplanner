#include "tflxmlhandler.h"
#include <QDebug>

Q_DECLARE_METATYPE(QXmlAttributes);

TFLXmlHandler::TFLXmlHandler() :
    QXmlDefaultHandler(),
    model(0),
    root(0),
    loc(0)
{
    initialiseValidTags();
    initialiseTagAssociations();
}

void TFLXmlHandler::initialiseValidTags()
{
    validTags.clear();
    validTags.append("itdRequest");
}

/* Set up which tags can lead to other tags */
void TFLXmlHandler::initialiseTagAssociations()
{
    tagAssociations.clear();
    tagAssociations.insert("itdRequest", "itdTripRequest");
    tagAssociations.insert("itdTripRequest", "itdItinerary");
    tagAssociations.insert("itdItinerary", "itdRouteList");
    tagAssociations.insert("itdRouteList", "itdRoute");
    tagAssociations.insert("itdRoute", "itdPartialRouteList");
    tagAssociations.insert("itdPartialRouteList", "itdPartialRoute");
    tagAssociations.insert("itdPartialRoute", "itdPoint");
    tagAssociations.insert("itdPartialRoute", "itdMeansOfTransport");
    tagAssociations.insert("itdPoint", "itdDateTime");
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

    root->removeRow(root->rowCount() - 1);
    QStandardItem *item = new QStandardItem("Search earlier ...");
    item->setFlags(Qt::ItemIsEnabled);
    root->appendRow(item);
    loc = root;

    ignoreTag.clear();

    return true;
}

bool TFLXmlHandler::endDocument()
{
    qDebug("Parsing ended");

    QStandardItem *item = new QStandardItem("Search later ...");
    item->setFlags(Qt::ItemIsEnabled);
    root->appendRow(item);

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
        item = new QStandardItem(qName);
        validTags = tagAssociations.values(qName);
    }
    else
    {
        ignoreTag = qName;
    }

    if (item)
    {
        item->setData(QVariant::fromValue<QXmlAttributes>(atts));
        item->setFlags(Qt::ItemIsEnabled);
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
