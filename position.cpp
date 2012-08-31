#include "position.h"
#include <QtLocation/QGeoPositionInfoSource>
#include <QtLocation/QGeoPositionInfo>
#include <QDebug>

QTM_USE_NAMESPACE

Position::Position(QObject *parent) :
    QObject(parent)
{
    source = QGeoPositionInfoSource::createDefaultSource(this);
    if (source)
    {
        qDebug("Position: Source found and connected");
        connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdated(QGeoPositionInfo)));
    }
}

void Position::positionUpdated(const QGeoPositionInfo &info)
{
    qDebug() << "Position: " << info;
    if (info.hasAttribute(QGeoPositionInfo::HorizontalAccuracy) &&
        info.attribute(QGeoPositionInfo::HorizontalAccuracy) < 100.0)
    {
        source->stopUpdates();
        emit positionObtained(info);
    }
}

void Position::updatePosition()
{
    if (source)
        source->startUpdates();
}

void Position::stopUpdates()
{
    if (source)
        source->stopUpdates();
}

QGeoPositionInfo Position::getLastPosition()
{
    return source->lastKnownPosition();
}
