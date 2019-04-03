#ifndef LIBRARYQA_STEP_H
#define LIBRARYQA_STEP_H

#include "../qt_eagle_xml_parser/eagle.h"
#include "../qt_eagle_xml_parser/eaglelayers.h"

class LibraryQA_StepResult
{
public:
    LibraryQA_StepResult();
    virtual ~LibraryQA_StepResult();

    /**
     * @brief The Severity enum
     */
    enum Severity {
        Warning,
        Error,
        Passed
    };
    Severity severity() const;
    void setSeverity(const Severity &severity);

    enum DescriptionType {
        Short,
        Long,
        Both
    };
    void setDescription(const QString & desription, DescriptionType type = Both);
    QString shortDescription() const;
    void setShortDescription(const QString &shortDescription);

    QString longDescription() const;
    void setLongDescription(const QString &longDescription);

    bool highlight() const;
    void setHighlight(bool highlight);

    virtual void fix() = 0;

    virtual bool fixable() const = 0;

private:
    Severity m_severity = Error;
    QString m_shortDescription, m_longDescription;
    bool m_highlight;
};

class LibraryQA_Check
{
public:
    LibraryQA_Check();
    LibraryQA_Check(const QString & name);
    virtual ~LibraryQA_Check();

    enum CheckTarget {
        CheckTarget_Symbol,
        CheckTarget_Package,
        CheckTarget_Deviceset
    };

    void fix();

    virtual CheckTarget target() const = 0;

    QString name() const;
    void reset();

    int resultCount() const;

    bool isEnabled() const;
    void setEnabled(bool isEnabled);

    QString code() const;

protected:
    QString m_name;
    QString m_code;
    QList<LibraryQA_StepResult*> m_results;
    bool m_enabled = false;
};

class SymbolQACheck : public LibraryQA_Check
{
public:
    SymbolQACheck() : LibraryQA_Check() {}
    SymbolQACheck(const QString& name) : LibraryQA_Check(name) {}
    CheckTarget target() const {return  CheckTarget_Symbol;}
    virtual bool checkSymbol(Symbol *symbol) = 0;
};

class PackageQACheck : public LibraryQA_Check
{
public:
    PackageQACheck() : LibraryQA_Check() {}
    PackageQACheck(const QString& name) : LibraryQA_Check(name) {}
    CheckTarget target() const {return  CheckTarget_Package;}
    virtual bool checkPackage(Package *package) = 0;
};

class DeviceSetQACheck : public LibraryQA_Check
{
public:
    DeviceSetQACheck() : LibraryQA_Check() {}
    DeviceSetQACheck(const QString& name) : LibraryQA_Check(name) {}
    CheckTarget target() const {return  CheckTarget_Deviceset;}
    virtual bool checkDeviceset(Deviceset *ds) = 0;
};

#endif // LIBRARYQA_STEP_H
