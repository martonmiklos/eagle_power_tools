#include "libraryqaresults.h"
#include "ui_libraryqaresults.h"

LibraryQAResults::LibraryQAResults(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LibraryQAResults)
{
    ui->setupUi(this);
}

LibraryQAResults::~LibraryQAResults()
{
    delete ui;
}
