#ifndef LIBRARYQAWIZARD_H
#define LIBRARYQAWIZARD_H

#include "eagle.h"
#include "libraryqawizardselectlibrarypage.h"
#include "libraryqaselectcomponents.h"
#include "libraryqaselectchecks.h"
#include "libraryqaresults.h"

#include <QWizard>

class LibraryQAWizard : public QWizard
{
    Q_OBJECT
public:
    LibraryQAWizard(QWidget *parent = nullptr);

    enum Pages {
        SelectLibrary,
        SelectChecks,
        SelectComponents,
        Results
    };

    Library *library() const;

protected:
    void closeEvent(QCloseEvent *event);

private:
    LibraryQAWizardSelectLibraryPage *m_libSelectPage = nullptr;
    LibraryQASelectChecks *m_checksSelectPage = nullptr;
    LibraryQASelectComponents *m_elementsSelectPage = nullptr;
    LibraryQAResults *m_resultsPage = nullptr;
    Library *m_library = nullptr;
    void performChecks();

private slots:
    void currentIdChangedSlot(int id);
};

#endif // LIBRARYQAWIZARD_H
