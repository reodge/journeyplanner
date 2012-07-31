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

    void initialiseTagAssociations();
    void initialiseValidTags();

    void setDecorations();
    QString resourceFromType(const QString type) const;
    QPixmap getRoutePixmap(const QStandardItem *item) const;

    /* Handler member function pointers */
    void (TFLXmlHandler::*startTagHandler)(const QString &, const QXmlAttributes &);
    void (TFLXmlHandler::*endTagHandler)(const QString &);

    void itdRequestStart(const QString &name, const QXmlAttributes &atts);
    void itdRequestEnd(const QString &name);

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
