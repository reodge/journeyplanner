#include "position.h"
#include <QtLocation/QGeoPositionInfoSource>
#include <QtLocation/QGeoPositionInfo>
#include <QDebug>

QTM_USE_NAMESPACE

Position::Position(QObject *parent) :
    QObject(parent)
{
    source = QGeoPositionInfoSource::createDefaultSource(this);
    if(source)
    {
        connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdated(QGeoPositionInfo)));
        source->startUpdates();
    }
}

Position::~Position()
{
}

void Position::positionUpdated(const QGeoPositionInfo &info)
{
    qDebug() << "Position Updated:" << info;
    //source->stopUpdates();
}
