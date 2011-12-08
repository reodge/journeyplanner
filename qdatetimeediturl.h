#ifndef QDATETIMEEDITURL_H
#define QDATETIMEEDITURL_H

#include <QDateTimeEdit>

class QDateTimeEditUrl : public QDateTimeEdit
{
    Q_OBJECT
public:
    explicit QDateTimeEditUrl(QWidget *parent = 0);

    QString toString(const QString& format) const;

    QString toUrlString() const;

signals:

public slots:
    void initDateTime();

};

#endif // QDATETIMEEDITURL_H
