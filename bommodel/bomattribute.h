#ifndef BOMATTRIBUTE_H
#define BOMATTRIBUTE_H

#include <QString>

class BoMAttribute
{
public:
    BoMAttribute();
    BoMAttribute(const QString & key, const QString & value = QString(""));
private:
    QString m_key;
    QString m_value;
};

#endif // BOMATTRIBUTE_H
