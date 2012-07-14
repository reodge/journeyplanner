#include "qdatetimeediturl.h"

QDateTimeEditUrl::QDateTimeEditUrl(QWidget *parent) :
    QDateTimeEdit(parent)
{
}

/* Sets date/time to current */
void QDateTimeEditUrl::initDateTime()
{
    this->setDateTime(QDateTime::currentDateTime());
}

QString QDateTimeEditUrl::toString(const QString& format) const
{
    return this->dateTime().toString(format);
}
