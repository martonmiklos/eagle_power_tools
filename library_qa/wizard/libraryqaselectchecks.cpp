#include "libraryqaselectchecks.h"
#include "ui_libraryqaselectchecks.h"

LibraryQASelectChecks::LibraryQASelectChecks(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LibraryQASelectChecks)
{
    ui->setupUi(this);
    this->setTitle(tr("Select checks to be performed"));

    m_model = new LibraryQASelectChecksModel(this);
    ui->treeViewChecks->setModel(m_model);
    ui->treeViewChecks->header()->setSectionResizeMode(LibraryQASelectElementsModel::ColElementName, QHeaderView::Stretch);
    ui->treeViewChecks->header()->setSectionResizeMode(LibraryQASelectElementsModel::ColChecked, QHeaderView::Interactive);
    ui->treeViewChecks->expandAll();
}

LibraryQASelectChecks::~LibraryQASelectChecks()
{
    delete ui;
}
