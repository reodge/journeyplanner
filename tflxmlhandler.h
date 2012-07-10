#ifndef TFLXMLHANDLER_H
#define TFLXMLHANDLER_H

#include <QXmlDefaultHandler>

class TFLXmlHandler : public QXmlDefaultHandler
{
private:
    bool new_route;
    int route_num;

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

    virtual bool fatalError(const QXmlParseException &exception);
};

#endif // TFLXMLHANDLER_H
