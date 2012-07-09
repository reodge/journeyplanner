#ifndef TFLXMLHANDLER_H
#define TFLXMLHANDLER_H

#include <QXmlDefaultHandler>

class TFLXmlHandler : public QXmlDefaultHandler
{
public:
    explicit TFLXmlHandler();

    virtual bool startDocument();

    virtual bool endDocument();

    virtual bool startElement(const QString &namespaceURI,
                              const QString &localName,
                              const QString &qName,
                              const QXmlAttributes &atts);

    virtual bool characters(const QString &ch);

    virtual bool endElement(const QString &namespaceURI,
                            const QString &localName,
                            const QString &qName);
};

#endif // TFLXMLHANDLER_H
