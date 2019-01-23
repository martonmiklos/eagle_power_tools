#include "libraryqaselectchecks.h"
#include "ui_libraryqaselectchecks.h"

LibraryQASelectChecks::LibraryQASelectChecks(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LibraryQASelectChecks)
{
    ui->setupUi(this);
    this->setTitle(tr("Select checks to be performed"));
}

LibraryQASelectChecks::~LibraryQASelectChecks()
{
    delete ui;
}
