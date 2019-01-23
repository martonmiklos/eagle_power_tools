#ifndef CHECKNAMESANDVALUES_H
#define CHECKNAMESANDVALUES_H

#include "libraryqa_step.h"

#include "qt_eagle_xml_parser/eagle.h"

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

class CheckNamesAndValues : public LibraryQA_Step
{
public:
    CheckNamesAndValues();
    ~CheckNamesAndValues();

    bool check(Library *lib);
    bool fix(Library *lib);

private:
    bool checkPackage(Packages *packages);
    bool checkSymbols(Symbols *symbols);

    bool m_checkNames, m_checkValues;
    bool m_checkSymbols, m_checkPackages;
};

#endif // CHECKNAMESANDVALUES_H
