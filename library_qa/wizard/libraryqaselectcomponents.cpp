#include "libraryqaselectcomponents.h"
#include "ui_libraryqaselectcomponents.h"

LibraryQASelectComponents::LibraryQASelectComponents(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LibraryQASelectComponents)
{
    ui->setupUi(this);
    this->setTitle(tr("Select the items to check"));

    m_model = new LibraryElementsModel(this);
}

LibraryQASelectComponents::~LibraryQASelectComponents()
{
    delete ui;
}

void LibraryQASelectComponents::setLibrary(Library *lib)
{
    m_model->setLibrary(lib);
    ui->treeViewComponentsToCheck->setModel(m_model);
}
