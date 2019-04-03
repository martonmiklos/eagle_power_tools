#ifndef CHECKPACKAGENAMESANDVALUES_H
#define CHECKPACKAGENAMESANDVALUES_H

#include "../libraryqacheck.h"

#include "eagle.h"

class CheckPackageNamesAndValuesResult : public LibraryQA_StepResult
{
public:
    enum Type {
        Value,
        Name
    };

    CheckPackageNamesAndValuesResult(Package *pkg, Type type, Text *text = nullptr);

    Text *text() const;
    void setText(Text *text);

    Type type() const;

    Package *pkg() const;
    void fix() override;
    bool fixable() const override;

private:
    Package *m_pkg = nullptr;
    Type m_type;
    Text *m_text = nullptr;
};

class CheckPackageNamesAndValues : public PackageQACheck
{
public:
    CheckPackageNamesAndValues();
    bool checkPackage(Package *package) override;
    bool fixPackage(Package *package, LibraryQA_StepResult *result);

private:
    bool m_checkNames, m_checkValues;
};

#endif // CHECKNAMESANDVALUES_H
