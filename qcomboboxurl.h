#ifndef QCOMBOBOXURL_H
#define QCOMBOBOXURL_H

#include <QComboBox>

class QComboBoxUrl : public QComboBox
{
    Q_OBJECT
public:
    explicit QComboBoxUrl(QWidget *parent = 0);

    QString toUrlString() const;

signals:

public slots:
    void checkForPostcode(const QString& str);
    void actOnActivation(const int index);

private:
    int iLastValueBeforeAutoPostcode;
};

#endif // QCOMBOBOXURL_H
