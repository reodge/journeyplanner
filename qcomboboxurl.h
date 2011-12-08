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

};

#endif // QCOMBOBOXURL_H
