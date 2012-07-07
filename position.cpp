#include "position.h"
#include <QtLocation/QGeoPositionInfoSource>
#include <QtLocation/QGeoPositionInfo>
#include <QDebug>

QTM_USE_NAMESPACE

Position::Position(QObject *parent) :
    QObject(parent)
{
    info = NULL;
    source = QGeoPositionInfoSource::createDefaultSource(this);
    if(source)
    {
        connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(positionUpdated(QGeoPositionInfo)));
    }
}

Position::~Position()
{
}

void Position::positionUpdated(const QGeoPositionInfo &info)
{
    qDebug() << "Position Updated:" << info;
    if(info.hasAttribute(QGeoPositionInfo::HorizontalAccuracy) &&
       info.attribute(QGeoPositionInfo::HorizontalAccuracy) < 100.0)
    {
        this->info = &info;
        source->stopUpdates();
        emit positionObtained();
    }
}

void Position::updatePosition()
{
    if(source)
        source->startUpdates();
}

void Position::stopUpdates()
{
    if(source)
        source->stopUpdates();
}

void Position::waitForPosition()
{
}
