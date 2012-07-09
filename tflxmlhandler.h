#ifndef TFLXMLHANDLER_H
#define TFLXMLHANDLER_H

#include <QXmlDefaultHandler>

class TFLXmlHandler : public QXmlDefaultHandler
{
public:
    explicit TFLXmlHandler();
    bool startElement(const QString &namespaceURI,
                      const QString &localName,
                      const QString &qName,
                      const QXmlAttributes &atts);
};

#endif // TFLXMLHANDLER_H
