#ifndef TFLXMLHANDLER_H
#define TFLXMLHANDLER_H

#include <QXmlDefaultHandler>
#include <QStandardItemModel>

class TFLXmlHandler : public QXmlDefaultHandler
{
private:
    QStandardItemModel *model;
    QStandardItem *root;
    QStandardItem *loc;
    QMultiHash<QString, QString> tagAssociations;
    QList<QString> validTags;
    QString ignoreTag;
    bool started;

    void setDecorations();
    void initialiseTagAssociations();
    void initialiseValidTags();

public:
    explicit TFLXmlHandler();

    void setModel(QStandardItemModel *model);
    void setRootItem(QStandardItem *item);

    bool startDocument();

    bool endDocument();

    bool startElement(const QString &namespaceURI,
                      const QString &localName,
                      const QString &qName,
                      const QXmlAttributes &atts);

    bool characters(const QString &ch);

    bool endElement(const QString &namespaceURI,
                    const QString &localName,
                    const QString &qName);

    bool fatalError(const QXmlParseException &exception);
};

#endif // TFLXMLHANDLER_H
