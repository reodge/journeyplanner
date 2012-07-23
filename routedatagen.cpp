#include "routedatagen.h"
#include "routeitinerary.h"
#include "routemodel.h"
#include <QUrl>
#include <QDesktopServices>
#include <QNetworkRequest>
#include <QXmlInputSource>
#include <QDebug>

#if defined(Q_WS_MAEMO_5)
#include <QtMaemo5>
#endif

RouteDataGen::RouteDataGen(QObject *parent) :
    QObject(parent),
    model(0)
{
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

void RouteDataGen::setModel(QStandardItemModel *model)
{
    this->model = model;
    root = QModelIndex();
}

void RouteDataGen::setRootIndex(const QModelIndex &index)
{
    root = index;
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
    url.append(typeIndexToString(model->item(0)->child(RouteModel::LAYOUT_FROM_TYPE)->data().toInt()));
    url.append("&type_destination=");
    url.append(typeIndexToString(model->item(0)->child(RouteModel::LAYOUT_TO_TYPE)->data().toInt()));
    url.append("&name_origin=");
    url.append(model->item(0)->child(RouteModel::LAYOUT_FROM_NAME)->text());
    url.append("&name_destination=");
    url.append(model->item(0)->child(RouteModel::LAYOUT_TO_NAME)->text());

    qDebug() << "Opening URL: " << url << endl;

    manager.get(QNetworkRequest(QUrl(url, QUrl::TolerantMode)));
}

QString RouteDataGen::typeIndexToString(const int i) const
{
    /* Turns current index from this combo box into a string to be added to a url. */
    switch(i)
    {
    case 0:
        return QString("stop");
    case 1:
        return QString("locator");
    case 2:
        return QString("address");
    case 3:
        return QString("poi");
    case 4:
        /* type to use for position data */
    default:
        return QString("");
    }
}
