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

    /* This is the full regexp, it's not necessary to use the whole thing, we just check the beginning */
    //QRegExp rx("^[A-Z]{1,2}\\d{1,2}[A-Z]?\\d[A-Z]{2}$");
    QRegExp rx("^[A-Z]{1,2}\\d{1,2}.*$");
    return rx.exactMatch(s);
}

void QComboBoxUrl::indexActivated(const int index)
{
        iLastValueBeforeAutoPostcode = index;
        if (index == 4)
        {
            qDebug() << "Selected Here!" << endl;
        }
}

/* Turns current index from this combo box into a string to be added to a url. */
QString QComboBoxUrl::toUrlString() const
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
    default:
        return QString("");
    }
}
