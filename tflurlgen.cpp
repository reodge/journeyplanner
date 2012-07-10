#include "tflurlgen.h"
#include <QUrl>
#include <QDesktopServices>
#include <QNetworkRequest>
#include <QXmlInputSource>
#include <QDebug>

TFLURLGen::TFLURLGen(QObject *parent) :
    QObject(parent)
{
    xmlReader.setContentHandler(&xmlHandler);
    connect(&(this->manager), SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadReady(QNetworkReply*)));
}

void TFLURLGen::downloadReady (QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Network error: " << reply->errorString();
        reply->deleteLater();
        return;
    }

    if (!this->xmlReader.parse(QXmlInputSource(reply)))
        qDebug() << "Parsing failed" << endl;

    reply->deleteLater();
}

/* Puts together the data and opens TFL website */
void TFLURLGen::openTFL(QString origin,
                        QString dest,
                        QString origin_type,
                        QString dest_type,
                        QString deparr,
                        QString datetime)
{
    Q_UNUSED (deparr);
    Q_UNUSED (datetime);

    QString url("http://journeyplanner.tfl.gov.uk/user/XML_TRIP_REQUEST2?language=en");

    url.append("&type_origin=");
    url.append(origin_type);
    url.append("&type_destination=");
    url.append(dest_type);
    url.append("&name_origin=");
    url.append(origin);
    url.append("&name_destination=");
    url.append(dest);

    qDebug() << "Opening URL: " << url << endl;

    manager.get(QNetworkRequest(QUrl(url, QUrl::TolerantMode)));
}
