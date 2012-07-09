#include "tflxmlhandler.h"
#include <QDebug>

TFLXmlHandler::TFLXmlHandler() :
    QXmlDefaultHandler()
{
}

bool TFLXmlHandler::startElement(const QString &namespaceURI,
                                 const QString &localName,
                                 const QString &qName,
                                 const QXmlAttributes &atts)
{
    qDebug() << "Read start tag: " << localName << endl;
    qDebug() << "Tag attributes: " << endl;

    for (int i = 0; i < atts.length(); i++)
    {
        qDebug() << atts.localName(i) << " = " << atts.value(i) << endl;
    }

    qDebug() << "---------------------------------------" << endl;

    return true;
}
