#ifndef ROUTEMODEL_H
#define ROUTEMODEL_H

#include <QStandardItemModel>
#include "position.h"
#include "routedatagen.h"

class RouteModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum formLayout
    {
        LAYOUT_FROM_NAME,
        LAYOUT_FROM_TYPE,
        LAYOUT_TO_NAME,
        LAYOUT_TO_TYPE,
        LAYOUT_DEPARR,
        LAYOUT_DATETIME,
        LAYOUT_MAX
    };

    explicit RouteModel(QObject *parent = 0);

    void getRoutes();
    void cancelRoutes();

signals:
    void dataFinished();

public slots:
    /* A hint to the model to either start looking for position early, or stop looking to save power.
       (state == true) => Probably will be wanted, start looking
       (state == false) => Can stop looking
     */
    void findPositionHint(bool state);

private:
    RouteDataGen data;
    Position pos;
};

#endif // ROUTEMODEL_H
