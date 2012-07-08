#include "tflurlgen.h"
#include <QUrl>
#include <QDesktopServices>
#include <QDebug>

TFLURLGen::TFLURLGen(QObject *parent) :
    QObject(parent)
{
}

/* Puts together the data and opens TFL website */
void TFLURLGen::openTFL(QString origin,
                        QString dest,
                        QString origin_type,
                        QString dest_type,
                        QString deparr,
                        QString datetime)
{
    QString url("http://journeyplanner.tfl.gov.uk/user/XSLT_TRIP_REQUEST2?language=en&ptOptionsActive=-1&sessionID=0");

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

    QDesktopServices::openUrl(QUrl(url,QUrl::TolerantMode));
}
