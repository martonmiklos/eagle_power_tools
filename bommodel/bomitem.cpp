#include "bomitem.h"

BoMItem::BoMItem()
{

}

BoMItem::BoMItem(const QString &name, const QString &value) :
    m_name(name), m_value(value)
{

}

void BoMItem::addAttribte(const BoMAttribute attr)
{
    m_attributes.append(attr);
}
