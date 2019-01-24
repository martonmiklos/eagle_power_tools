#ifndef LIBRARYQA_STEP_H
#define LIBRARYQA_STEP_H

#include "../qt_eagle_xml_parser/eagle.h"

class LibraryQA_StepResult
{
public:
    LibraryQA_StepResult();

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
    virtual ~LibraryQA_Check() = 0;

    enum CheckTarget {
        CheckTarget_Symbol,
        CheckTarget_Package,
        CheckTarget_Deviceset
    };

    virtual bool checkPackage(Package *package) = 0;
    virtual bool checkSymbol(Symbol *symbol) = 0;
    virtual bool checkDeviceset(Deviceset *ds) = 0;

    virtual bool fixPackage(Package *package, LibraryQA_StepResult *result) = 0;
    virtual bool fixSymbol(Symbol *symbol, LibraryQA_StepResult *result) = 0;
    virtual bool fixDeviceset(Deviceset *ds, LibraryQA_StepResult *result) = 0;

    virtual QList<CheckTarget> targets() const = 0;


    QString name() const;

protected:
    QString m_name;
};

#endif // LIBRARYQA_STEP_H
