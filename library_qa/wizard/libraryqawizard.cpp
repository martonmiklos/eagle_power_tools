#include "libraryqawizard.h"

LibraryQAWizard::LibraryQAWizard(QWidget *parent) :
    QWizard (parent)
{
    m_libSelectPage = new LibraryQAWizardSelectLibraryPage(this);
    addPage(m_libSelectPage);

    m_checksSelectPage = new LibraryQASelectChecks(this);
    addPage(m_checksSelectPage);

    m_elementsSelectPage = new LibraryQASelectComponents(this);
    addPage(m_elementsSelectPage);

    m_resultsPage = new LibraryQAResults(this);
    addPage(m_resultsPage);

    connect(this, &LibraryQAWizard::currentIdChanged, this, &LibraryQAWizard::currentIdChangedSlot);
}

void LibraryQAWizard::closeEvent(QCloseEvent *event)
{
    QWizard::closeEvent(event);
    // TODO free the loaded eagle design
}

Library *LibraryQAWizard::library() const
{
    return m_library;
}

void LibraryQAWizard::currentIdChangedSlot(int id)
{
    switch (static_cast<Pages>(id)) {
    case LibraryQAWizard::SelectLibrary:
        break;
    case LibraryQAWizard::SelectChecks:
        break;
    case LibraryQAWizard::SelectComponents:
        m_library = m_libSelectPage->eagle()->drawing()->library();
        m_elementsSelectPage->setLibrary(m_library);
        break;
    case LibraryQAWizard::Results:
        break;
    default:
        break;
    }
}
