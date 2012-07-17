#include "route.h"
#include <assert.h>
#include <QString>

Route::Route()
{
}

void Route::setAttr(const QString &key, const long long &val)
{
    atts.insert(key, val);
}

long long Route::getAttr(const QString &key)
{
    QList<long long> l = atts.values(key);

    assert(l.count() == 1 && "Multiple values for single attribute");

    return l[0];
}
