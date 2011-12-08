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

QString QDateTimeEditUrl::toUrlString() const
{
    QString url("");

    url.append("&itdDateDay=");
    url.append(this->toString("d"));
    url.append("&itdDateYearMonth=");
    url.append(this->toString("yyyyMM"));
    url.append("&itdTimeHour=");
    url.append(this->toString("h"));
    url.append("&itdTimeMinute=");
    url.append(this->toString("m"));

    return url;
}
