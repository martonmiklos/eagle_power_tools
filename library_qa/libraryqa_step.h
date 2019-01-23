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

class LibraryQA_Step
{
public:
    LibraryQA_Step();
    virtual ~LibraryQA_Step() = 0;

    virtual bool check(Library *library) = 0;
    virtual bool fix(Library *library) = 0;
    QList<LibraryQA_StepResult *> results() const;

protected:
    QList<LibraryQA_StepResult*> m_results;
};

#endif // LIBRARYQA_STEP_H
