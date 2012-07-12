#include "routedatagen.h"
#include "rawdata.h"
#include <QUrl>
#include <QDesktopServices>
#include <QNetworkRequest>
#include <QXmlInputSource>
#include <QDebug>

#if defined(Q_WS_MAEMO_5)
#include <QtMaemo5>
#endif

RouteDataGen::RouteDataGen(RawData *data, QObject *parent) :
    QObject(parent)
{
    this->data = data;
    xmlReader.setContentHandler(&xmlHandler);
    connect(&(this->manager), SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadReady(QNetworkReply*)));
    //connect(&(this->xmlHandler), SIGNAL(routesReady(RouteItinerary*)), parent, SLOT(routeDataReady(RouteItinerary*)));
}

void RouteDataGen::downloadReady (QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        QString errorString(reply->errorString());

        qDebug() << errorString;

        #if defined(Q_WS_MAEMO_5)
            QMaemo5InformationBox::information(NULL, errorString, QMaemo5InformationBox::NoTimeout);
        #endif

        reply->deleteLater();
        return;
    }

    if (!this->xmlReader.parse(QXmlInputSource(reply)))
        qDebug() << "Parsing failed" << endl;

    reply->deleteLater();
}

/* Puts together the data and opens TFL website */
void RouteDataGen::openTFL(QString dest,
                           QString origin_type,
                           QString dest_type,
                           QString deparr,
                           QString datetime)
{
    Q_UNUSED(deparr);
    Q_UNUSED(datetime);

    QString url("http://journeyplanner.tfl.gov.uk/user/XML_TRIP_REQUEST2?language=en");

    url.append("&type_origin=");
    url.append(origin_type);
    url.append("&type_destination=");
    url.append(dest_type);
    url.append("&name_origin=");
    url.append(data->getOrigin());
    url.append("&name_destination=");
    url.append(dest);

    qDebug() << "Opening URL: " << url << endl;

    manager.get(QNetworkRequest(QUrl(url, QUrl::TolerantMode)));
}
