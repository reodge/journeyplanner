#ifndef TFLURLGEN_H
#define TFLURLGEN_H

#include <QObject>
#include <QtGui>
#include <QPersistentModelIndex>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlSimpleReader>
#include "tflxmlhandler.h"

class RouteDataGen : public QObject
{
    Q_OBJECT
public:
    explicit RouteDataGen(QObject *parent = 0);
    void getData();
    void setModel(QStandardItemModel *model);
    void setRootItem(QStandardItem *item);

signals:
    void dataFinished();

private slots:
    void downloadReady (QNetworkReply *reply);

private:
    QNetworkAccessManager manager;
    TFLXmlHandler xmlHandler;
    QXmlSimpleReader xmlReader;
    QStandardItemModel *model;
    QStandardItem *root;

    QString getBaseTFLURL();
    QString typeIndexToString(const int i) const;
};

#endif // TFLURLGEN_H
