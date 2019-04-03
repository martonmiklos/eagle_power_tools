#ifndef SYMBOLPINGRIDCHECK_H
#define SYMBOLPINGRIDCHECK_H

#include "../libraryqacheck.h"

class SymbolPinGridCheckResult : public LibraryQA_StepResult
{
public:
    enum FixDirection {
        X,
        Y
    };
    SymbolPinGridCheckResult(Pin *pin, FixDirection direction, qreal offset);
    void fix();
    bool fixable() const {return true;}

private:
    Pin *m_pin;
    qreal m_offset = 0.0;
    FixDirection m_offsetDirection = X;
};

class SymbolPinGridCheck : public SymbolQACheck
{
public:
    SymbolPinGridCheck();
    bool checkSymbol(Symbol *symbol) override;
};

#endif // SYMBOLPINGRIDCHECK_H
