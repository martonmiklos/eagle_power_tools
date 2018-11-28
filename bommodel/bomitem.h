#ifndef BOMITEM_H
#define BOMITEM_H

#include "bomattribute.h"

#include <QList>

class BoMItem
{
public:
    BoMItem();
    BoMItem(const QString & name, const QString &value = QString(""));

    void addAttribte(const BoMAttribute attr);

private:
    QList <BoMAttribute> m_attributes;
    QString m_name;
    QString m_value;
};

#endif // BOMITEM_H
