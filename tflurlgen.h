#ifndef TFLURLGEN_H
#define TFLURLGEN_H

#include "tflxmlhandler.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlSimpleReader>

class TFLURLGen : public QObject
{
    Q_OBJECT
public:
    explicit TFLURLGen(QObject *parent = 0);
    void openTFL(QString origin,
                 QString dest,
                 QString origin_type,
                 QString dest_type,
                 QString deparr,
                 QString datetime);
    
signals:
    
public slots:
    void downloadReady (QNetworkReply *reply);

private:
    QNetworkAccessManager manager;
    TFLXmlHandler xmlHandler;
    QXmlSimpleReader xmlReader;

    QString getBaseTFLURL();
};

#endif // TFLURLGEN_H
