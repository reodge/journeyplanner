#include "tflxmlhandler.h"
#include <QDebug>

TFLXmlHandler::TFLXmlHandler() :
    QXmlDefaultHandler(),
    model(0),
    root(0)
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

    root->removeRow(root->rowCount() - 1);
    QStandardItem *item = new QStandardItem("Search earlier ...");
    item->setFlags(Qt::ItemIsEnabled);
    root->appendRow(item);
    loc = root;

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

    if (qName == "itdRoute")
    {
        QStandardItem *item = new QStandardItem("New Route");
        item->setFlags(Qt::ItemIsEnabled);
        loc->appendRow(item);
        loc = item;
    }
    else if (qName == "")
    {
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
    {
        loc = loc->parent();
    }
    else if (qName == "")
    {
    }

    return true;
}

bool TFLXmlHandler::fatalError(const QXmlParseException &exception)
{
    qDebug() << "Parsing failed: " << exception.message() << endl <<
                " Line: " << exception.lineNumber() << ", Column: " << exception.columnNumber() << endl;

    return false;
}
