#include "routedatagen.h"
#include "routeitinerary.h"
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
    {
        qDebug() << "Parsing failed" << endl;
    }
    else
    {
        /* We need to figure out how to clean this up ... we currently rely on the
           dataReady signal getting connected to a slot which deletes ri.
         */
        RouteItinerary *ri;

        if (xmlHandler.getRoutes(ri))
            emit dataReady(ri);
        else
            qDebug() << "Parsing succeeded, but no routes." << endl;
    }

    reply->deleteLater();
}

/* Puts together the data and opens TFL website */
void RouteDataGen::getData()
{
    QString url("http://journeyplanner.tfl.gov.uk/user/XML_TRIP_REQUEST2?language=en");

    /* Useful stuff to add later */
    //QString url("&itdTripDateTimeDepArr=");
    //url.append("&itdDateDay=");
    //url.append(this->toString("d"));
    //url.append("&itdDateYearMonth=");
    //url.append(this->toString("yyyyMM"));
    //url.append("&itdTimeHour=");
    //url.append(this->toString("h"));
    //url.append("&itdTimeMinute=");
    //url.append(this->toString("m"));

    url.append("&type_origin=");
    url.append(data->getOriginType());
    url.append("&type_destination=");
    url.append(data->getDestType());
    url.append("&name_origin=");
    url.append(data->getOrigin());
    url.append("&name_destination=");
    url.append(data->getDest());

    qDebug() << "Opening URL: " << url << endl;

    manager.get(QNetworkRequest(QUrl(url, QUrl::TolerantMode)));
}
