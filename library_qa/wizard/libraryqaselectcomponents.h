#ifndef LIBRARYQASELECTCOMPONENTS_H
#define LIBRARYQASELECTCOMPONENTS_H

#include "models/libraryelementssmodel.h"

#include <QWizardPage>

namespace Ui {
class LibraryQASelectComponents;
}

class LibraryQASelectComponents : public QWizardPage
{
    Q_OBJECT

public:
    explicit LibraryQASelectComponents(QWidget *parent = nullptr);
    ~LibraryQASelectComponents();
    void setLibrary(Library *lib);
private:
    Ui::LibraryQASelectComponents *ui;
    LibraryElementsModel *m_model = nullptr;
};

#endif // LIBRARYQASELECTCOMPONENTS_H
