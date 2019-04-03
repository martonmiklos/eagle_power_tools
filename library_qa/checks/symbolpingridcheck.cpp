#include "symbolpingridcheck.h"

#include <qmath.h>

SymbolPinGridCheck::SymbolPinGridCheck() :
    SymbolQACheck (QObject::tr("Check symbol pin grid positions"))
{
    m_code = "symbol_pin_grid_check";
}

bool SymbolPinGridCheck::checkSymbol(Symbol *symbol)
{
    bool ret = true;
    for (Pin *pin : *symbol->pinList()) {
        qreal offset = fmod(pin->x(), 2.54);
        if (!qFuzzyIsNull(offset)) {
            ret = false;
            auto *res = new SymbolPinGridCheckResult(pin,
                                                     SymbolPinGridCheckResult::X,
                                                     offset);
            m_results.append(res);
        }

        offset = fmod(pin->y(), 2.54);
        if (!qFuzzyIsNull(offset)) {
            ret = false;
            auto *res = new SymbolPinGridCheckResult(pin,
                                                     SymbolPinGridCheckResult::Y,
                                                     offset);
            m_results.append(res);
        }
    }
    return ret;
}

SymbolPinGridCheckResult::SymbolPinGridCheckResult(Pin *pin, FixDirection direction, qreal offset) :
    m_pin(pin),
    m_offset(offset),
    m_offsetDirection(direction)
{
    if (direction) {
        setDescription(QObject::tr("The %1 pin's X coordinate is not on 0.1 inch grid")
                       .arg(pin->name()), DescriptionType::Short);
        setDescription(QObject::tr("The %1 pin's X coordinate is not on 0.1 inch grid: %2")
                       .arg(pin->name())
                       .arg(offset), DescriptionType::Long);
    } else {
        setDescription(QObject::tr("The %1 pin's Y coordinate is not on 0.1 inch grid")
                       .arg(pin->name()), DescriptionType::Short);
        setDescription(QObject::tr("The %1 pin's Y coordinate is not on 0.1 inch grid: %2")
                       .arg(pin->name())
                       .arg(offset), DescriptionType::Long);
    }
}

void SymbolPinGridCheckResult::fix()
{
    if (m_offsetDirection == X) {
        if (m_offset < 0.127) {
            m_pin->setX(m_pin->x() - m_offset);
        } else {
            m_pin->setX(m_pin->x() + (2.54 - m_offset));
        }
    } else {
        if (m_offset < 0.127) {
            m_pin->setX(m_pin->y() - m_offset);
        } else {
            m_pin->setX(m_pin->y() + (2.54 - m_offset));
        }
    }
}
