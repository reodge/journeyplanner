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
    ~Position();

    void updatePosition();

    void waitForPosition();

signals:
    void positionObtained();

private slots:
    void positionUpdated(const QGeoPositionInfo& info);

private:
    QGeoPositionInfoSource *source;
    const QGeoPositionInfo *info;
};

#endif // POSITION_H
