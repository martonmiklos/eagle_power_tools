#ifndef CHECKNAMESANDVALUES_H
#define CHECKNAMESANDVALUES_H

#include "../libraryqacheck.h"

#include "eagle.h"

class CheckSymbolNamesAndValuesResult : public LibraryQA_StepResult
{
public:
    enum Type {
        Value,
        Name
    };

    CheckSymbolNamesAndValuesResult(Symbol *symbol, Type type, Text *text = nullptr);

    void fix() override;
    bool fixable() const override;
private:
    Symbol *m_symbol = nullptr;
    Type m_type;
    Text *m_text = nullptr;
};

class CheckSymbolNamesAndValues : public SymbolQACheck
{
public:
    CheckSymbolNamesAndValues();

    bool checkSymbol(Symbol *symbol);
    bool fixSymbol(Symbol *symbol, LibraryQA_StepResult *result);

private:
    bool m_checkNames, m_checkValues;
};

#endif // CHECKNAMESANDVALUES_H
