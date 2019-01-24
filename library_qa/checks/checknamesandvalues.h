#ifndef CHECKNAMESANDVALUES_H
#define CHECKNAMESANDVALUES_H

#include "../libraryqacheck.h"

#include "eagle.h"

class CheckNamesAndValuesResult : public LibraryQA_StepResult
{
public:
    enum Type {
        Value,
        Name
    };

    CheckNamesAndValuesResult(Type type, Text *text = nullptr);

    Text *text() const;
    void setText(Text *text);

    Type type() const;

    Package *pkg() const;
    void setPkg(Package *pkg);

    Symbol *symbol() const;
    void setSymbol(Symbol *symbol);

private:
    Package *m_pkg = nullptr;
    Symbol *m_symbol = nullptr;
    Type m_type;
    Text *m_text = nullptr;
};

class CheckNamesAndValues : public LibraryQA_Check
{
public:
    CheckNamesAndValues();
    virtual ~CheckNamesAndValues();

    bool checkPackage(Package *package);
    bool checkSymbol(Symbol *symbol);
    bool checkDeviceset(Deviceset *ds) {return false;}

    bool fixPackage(Package *package, LibraryQA_StepResult *result);
    bool fixSymbol(Symbol *symbol, LibraryQA_StepResult *result);
    bool fixDeviceset(Deviceset *ds, LibraryQA_StepResult *result) {return false;}

    QList<CheckTarget> targets() const;

private:
    bool m_checkNames, m_checkValues;
    bool m_checkSymbols, m_checkPackages;

    bool fix(CheckNamesAndValuesResult *result);
};

#endif // CHECKNAMESANDVALUES_H
