#ifndef ROUTEDELEGATE_H
#define ROUTEDELEGATE_H

#include <QStyledItemDelegate>

class RouteDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit RouteDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    
signals:
    
public slots:
    
};

#endif // ROUTEDELEGATE_H
