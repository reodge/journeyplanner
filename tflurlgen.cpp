#include "tflurlgen.h"
#include <QUrl>
#include <QDesktopServices>
#include <QNetworkRequest>
#include <QXmlInputSource>
#include <QDebug>

TFLURLGen::TFLURLGen(QObject *parent) :
    QObject(parent)
{
    connect(&(this->manager), SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadReady(QNetworkReply*)));
}

void TFLURLGen::downloadReady (QNetworkReply *reply)
{
    bool parsed = this->xmlReader.parse(QXmlInputSource(reply));

    if (!parsed)
    {
        qDebug() << "Parsing failed" << endl;
        /* TODO Show to user? */
    }
}

/* Puts together the data and opens TFL website */
void TFLURLGen::openTFL(QString origin,
                        QString dest,
                        QString origin_type,
                        QString dest_type,
                        QString deparr,
                        QString datetime)
{
    QString url("http://journeyplanner.tfl.gov.uk/user/XML_TRIP_REQUEST2?language=en&ptOptionsActive=-1&sessionID=0");

    url.append("&type_origin=");
    url.append(origin_type);
    url.append("&type_destination=");
    url.append(dest_type);
    url.append(deparr);
    url.append(datetime);
    url.append("&name_origin=");
    url.append(origin);
    url.append("&name_destination=");
    url.append(dest);

    qDebug() << "Opening URL: " << url << endl;

    //QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
    manager.get(QNetworkRequest(QUrl(url, QUrl::TolerantMode)));
}
