#ifndef LIBRARYQASELECTCHECKS_H
#define LIBRARYQASELECTCHECKS_H

#include <QWizardPage>

#include "library_qa/wizard/models/libraryqachecksmodel.h"

namespace Ui {
class LibraryQASelectChecks;
}

class LibraryQASelectChecks : public QWizardPage
{
    Q_OBJECT

public:
    explicit LibraryQASelectChecks(QWidget *parent = nullptr);
    ~LibraryQASelectChecks();
    QList<LibraryQA_Check*> selectedChecks() const;
private:
    Ui::LibraryQASelectChecks *ui;
    LibraryQASelectChecksModel *m_model = nullptr;
};

#endif // LIBRARYQASELECTCHECKS_H
