#include "qcomboboxurl.h"

QComboBoxUrl::QComboBoxUrl(QWidget *parent) :
    QComboBox(parent)
{
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
