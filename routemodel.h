#ifndef ROUTEMODEL_H
#define ROUTEMODEL_H

#include <QStandardItemModel>

class RouteModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit RouteModel(QObject *parent = 0);

    void getRoutes();

signals:

public slots:

};

#endif // ROUTEMODEL_H
