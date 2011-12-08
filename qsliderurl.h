#ifndef QSLIDERURL_H
#define QSLIDERURL_H

#include <QSlider>

class QSliderUrl : public QSlider
{
    Q_OBJECT
public:
    explicit QSliderUrl(QWidget *parent = 0);

    QString toUrlString() const;

signals:

public slots:

};

#endif // QSLIDERURL_H
