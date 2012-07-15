#ifndef TFLURLGEN_H
#define TFLURLGEN_H

#include "tflxmlhandler.h"
#include "rawdata.h"
#include "routeitinerary.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlSimpleReader>

class RouteDataGen : public QObject
{
    Q_OBJECT
public:
    explicit RouteDataGen(RawData *data, QObject *parent = 0);
    void getData();
    
signals:
    void dataReady(RouteItinerary *itinerary);
    
private slots:
    void downloadReady (QNetworkReply *reply);

private:
    RawData *data;
    QNetworkAccessManager manager;
    TFLXmlHandler xmlHandler;
    QXmlSimpleReader xmlReader;

    QString getBaseTFLURL();
};

#endif // TFLURLGEN_H
