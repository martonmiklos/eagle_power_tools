#ifndef LIBRARYQASELECTCHECKS_H
#define LIBRARYQASELECTCHECKS_H

#include <QWizardPage>

namespace Ui {
class LibraryQASelectChecks;
}

class LibraryQASelectChecks : public QWizardPage
{
    Q_OBJECT

public:
    explicit LibraryQASelectChecks(QWidget *parent = nullptr);
    ~LibraryQASelectChecks();

private:
    Ui::LibraryQASelectChecks *ui;
};

#endif // LIBRARYQASELECTCHECKS_H
