#ifndef POSITION_H
#define POSITION_H

#include <QtLocation/QGeoPositionInfoSource>
#include <QtLocation/QGeoPositionInfo>
#include <QtLocation/QGeoCoordinate>

QTM_USE_NAMESPACE

class Position : public QObject
{
    Q_OBJECT
public:
    explicit Position(QObject *parent = 0);

    void updatePosition();
    void stopUpdates();
    QGeoPositionInfo getLastPosition();

signals:
    void positionObtained(QGeoPositionInfo info);

private slots:
    void positionUpdated(const QGeoPositionInfo &info);

private:
    QGeoPositionInfoSource *source;
};

#endif // POSITION_H
