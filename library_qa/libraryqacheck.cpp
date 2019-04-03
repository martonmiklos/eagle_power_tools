#include "libraryqacheck.h"

LibraryQA_Check::LibraryQA_Check()
{

}

LibraryQA_Check::LibraryQA_Check(const QString &name) :
    m_name(name)
{

}

void LibraryQA_Check::fix()
{
    for (auto *r : m_results) {
        r->fix();
    }
}

QString LibraryQA_Check::name() const
{
    return m_name;
}

void LibraryQA_Check::reset()
{
    qDeleteAll(m_results);
}

int LibraryQA_Check::resultCount() const
{
    return m_results.count();
}

bool LibraryQA_Check::isEnabled() const
{
    return m_enabled;
}

void LibraryQA_Check::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

QString LibraryQA_Check::code() const
{
    return m_code;
}

LibraryQA_Check::~LibraryQA_Check()
{
    reset();
}


LibraryQA_StepResult::LibraryQA_StepResult()
{

}

LibraryQA_StepResult::~LibraryQA_StepResult()
{

}

LibraryQA_StepResult::Severity LibraryQA_StepResult::severity() const
{
    return m_severity;
}

void LibraryQA_StepResult::setSeverity(const Severity &severity)
{
    m_severity = severity;
}

void LibraryQA_StepResult::setDescription(const QString &description, LibraryQA_StepResult::DescriptionType type)
{
    switch (type) {
    case LibraryQA_StepResult::Short:
        m_shortDescription = description;
        break;
    case LibraryQA_StepResult::Long:
        m_longDescription = description;
        break;
    case LibraryQA_StepResult::Both:
        m_longDescription = m_shortDescription = description;
        break;
    }
}

QString LibraryQA_StepResult::shortDescription() const
{
    return m_shortDescription;
}

void LibraryQA_StepResult::setShortDescription(const QString &shortDescription)
{
    m_shortDescription = shortDescription;
}

QString LibraryQA_StepResult::longDescription() const
{
    return m_longDescription;
}

void LibraryQA_StepResult::setLongDescription(const QString &longDescription)
{
    m_longDescription = longDescription;
}

bool LibraryQA_StepResult::highlight() const
{
    return m_highlight;
}

void LibraryQA_StepResult::setHighlight(bool highlight)
{
    m_highlight = highlight;
}
