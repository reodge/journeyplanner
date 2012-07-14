#include "qcomboboxurl.h"
#include <QRegExp>
#include <QDebug>

QComboBoxUrl::QComboBoxUrl(QWidget *parent) :
    QComboBox(parent),
    iLastValueBeforeAutoPostcode(0)
{
}

void QComboBoxUrl::checkForPostcode(const QString& str)
{
    if(isPostcode(str))
    {
        /* Only set the last index value if we are currently not already on 'Postcode' */
        if(this->currentIndex() != 1)
            iLastValueBeforeAutoPostcode = this->currentIndex();
        this->setCurrentIndex(1);
    }
    else
    {
        this->setCurrentIndex(iLastValueBeforeAutoPostcode);
    }
}

bool QComboBoxUrl::isPostcode(QString str)
{
    QString s = str.remove(' ').toUpper();

    /* Full regex for postcode. */
    QRegExp rx("^[A-Z]{1,2}\\d{1,2}[A-Z]?\\d[A-Z]{2}$");

    /* This is the regex for just the first half, in case we want to use it instead. */
    //QRegExp rx("^[A-Z]{1,2}\\d{1,2}.*$");

    return rx.exactMatch(s);
}

void QComboBoxUrl::indexActivated(const int index)
{
        iLastValueBeforeAutoPostcode = index;
}

/* Turns current index from this combo box into a string to be added to a url. */
QString QComboBoxUrl::toString() const
{
    switch(this->currentIndex())
    {
    case 0:
        return QString("stop");
    case 1:
        return QString("locator");
    case 2:
        return QString("address");
    case 3:
        return QString("poi");
    case 4:
        /* type to use for position data */
    default:
        return QString("");
    }
}
