#include "tflxmlhandler.h"
#include <QDebug>

TFLXmlHandler::TFLXmlHandler() :
    QXmlDefaultHandler()
{
}

bool TFLXmlHandler::startDocument()
{
    qDebug() << "Parsing started";

    return true;
}

bool TFLXmlHandler::endDocument()
{
    qDebug() << "Parsing ended";

    return true;
}

bool TFLXmlHandler::startElement(const QString &namespaceURI,
                                 const QString &localName,
                                 const QString &qName,
                                 const QXmlAttributes &atts)
{
    qDebug() << "---<"<< localName << ">-----------------------";

    for (int i = 0; i < atts.length(); i++)
    {
        qDebug() << "  " << atts.localName(i) << " = " << atts.value(i);
    }

    return true;
}

bool TFLXmlHandler::characters(const QString &ch)
{
    qDebug() << ch;

    return true;
}

bool TFLXmlHandler::endElement(const QString &namespaceURI,
                               const QString &localName,
                               const QString &qName)
{
    qDebug() << "---</"<< localName << ">----------------------";

    return true;
}
