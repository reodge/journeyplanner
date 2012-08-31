#include "routedatagen.h"
#include "routemodel.h"
#include <QUrl>
#include <QDesktopServices>
#include <QNetworkRequest>
#include <QXmlInputSource>
#include <QtLocation/QGeoPositionInfo>
#include <QDebug>

#if defined(Q_WS_MAEMO_5)
#include <QtMaemo5>
#endif

QTM_USE_NAMESPACE

RouteDataGen::RouteDataGen(QObject *parent) :
    QObject(parent),
    model(0),
    root(0),
    currentNetworkRequest(0)
{
    xmlReader.setContentHandler(&xmlHandler);
    connect(&(this->manager), SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadReady(QNetworkReply*)));
}

void RouteDataGen::downloadReady (QNetworkReply *reply)
{
    currentNetworkRequest = 0;
    emit dataFinished();

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

    reply->deleteLater();
}

void RouteDataGen::setModel(QStandardItemModel *model)
{
    this->model = model;
    root = model->invisibleRootItem();

    xmlHandler.setModel(model);
    xmlHandler.setRootItem(root);
}

void RouteDataGen::setRootItem(QStandardItem *item)
{
    root = item;
    xmlHandler.setRootItem(root);
}

void RouteDataGen::setPosition(const QGeoPositionInfo &info)
{
    this->info = info;
}

void RouteDataGen::cancelData()
{
    if (currentNetworkRequest)
        currentNetworkRequest->abort();

    currentNetworkRequest = 0;
}

/* Puts together the data and opens TFL website */
void RouteDataGen::getData()
{ QString url("http://journeyplanner.tfl.gov.uk/user/XML_TRIP_REQUEST2?language=en");

    int fromType = model->item(0)->child(RouteModel::LAYOUT_FROM_TYPE)->text().toInt();
    int toType = model->item(0)->child(RouteModel::LAYOUT_TO_TYPE)->text().toInt();
    QString fromText = model->item(0)->child(RouteModel::LAYOUT_FROM_NAME)->text();
    QString toText = model->item(0)->child(RouteModel::LAYOUT_TO_NAME)->text();
    bool fromIsPosition = (fromType == 4);
    bool toIsPosition = (toType == 4);

    url.append("&type_origin=");
    url.append(typeIndexToString(fromType));
    url.append("&type_destination=");
    url.append(typeIndexToString(toType));

    url.append("&name_origin=");
    if (fromIsPosition)
        url.append(coordToString(info.coordinate()));
    else
        url.append(fromText);

    url.append("&name_destination=");
    if (toIsPosition)
        url.append(coordToString(info.coordinate()));
    else
        url.append(toText);

    url.append("&itdTripDateTimeDepArr=");

    switch (model->item(0)->child(RouteModel::LAYOUT_DEPARR)->text().toInt())
    {
    case 0:
        url.append("dep");
        break;
    case 1:
        url.append("arr");
        break;
    default:
        break;
    }

    QDateTime dt = QDateTime::fromString(model->item(0)->child(RouteModel::LAYOUT_DATETIME)->text(), Qt::ISODate);
    qDebug() << "Date/time =" << dt;

    url.append(dt.toString("'&itdDate='yyMMdd'&itdTime='hhmm'h'"));

    qDebug() << "Opening URL: " << url << endl;

    QStandardItem *item = new QStandardItem("Loading data ...");
    item->setFlags(Qt::ItemIsEnabled);
    root->removeRows(0, root->rowCount());
    root->appendRow(item);

    currentNetworkRequest = manager.get(QNetworkRequest(QUrl(url, QUrl::TolerantMode)));
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
        return QString("coord");
    default:
        return QString("");
    }
}

QString RouteDataGen::coordToString(const QGeoCoordinate &coord)
{
    return QString("%1:%2:WGS84[DD.ddddd]").arg(coord.longitude(), 0, 'f').arg(coord.latitude(), 0, 'f');
}
