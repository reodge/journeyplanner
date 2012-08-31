#ifndef TFLURLGEN_H
#define TFLURLGEN_H

#include <QObject>
#include <QtGui>
#include <QPersistentModelIndex>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlSimpleReader>
#include <QtLocation/QGeoPositionInfo>
#include "tflxmlhandler.h"

QTM_USE_NAMESPACE

class RouteDataGen : public QObject
{
    Q_OBJECT
public:
    explicit RouteDataGen(QObject *parent = 0);
    void getData();
    void setModel(QStandardItemModel *model);
    void setRootItem(QStandardItem *item);
    void setPosition(const QGeoPositionInfo &info);
    void cancelData();

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
    QGeoPositionInfo info;
    QNetworkReply *currentNetworkRequest;

    QString getBaseTFLURL();
    QString typeIndexToString(const int i) const;
    QString coordToString(const QGeoCoordinate &coord);
};

#endif // TFLURLGEN_H
