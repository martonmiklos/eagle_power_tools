#include "libraryqawizardselectlibrarypage.h"
#include "ui_libraryqawizardselectlibrarypage.h"

#include <QMessageBox>

LibraryQAWizardSelectLibraryPage::LibraryQAWizardSelectLibraryPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LibraryQAWizardSelectLibraryPage)
{
    ui->setupUi(this);
    setTitle(tr("Select library"));
    m_settings.beginGroup("libraryQA");
    ui->patheditLibrary->setPath(m_settings.value("lastLibraryPath").toString());
    m_settings.endGroup();
}

LibraryQAWizardSelectLibraryPage::~LibraryQAWizardSelectLibraryPage()
{
    delete ui;
}

bool LibraryQAWizardSelectLibraryPage::isComplete() const
{
    return !ui->patheditLibrary->path().isEmpty();
}

void LibraryQAWizardSelectLibraryPage::on_patheditLibrary_pathChanged(const QString &path)
{
    emit completeChanged();
    if (!path.isEmpty()) {
        m_settings.beginGroup("libraryQA");
        m_settings.setValue("lastLibraryPath", path);
        m_settings.endGroup();
    }
}

Eagle *LibraryQAWizardSelectLibraryPage::eagle() const
{
    return m_eagle;
}

void LibraryQAWizardSelectLibraryPage::setEagle(Eagle *eagle)
{
    m_eagle = eagle;
}

bool LibraryQAWizardSelectLibraryPage::validatePage()
{
    if (ui->patheditLibrary->path().isEmpty())
        return false;
    bool ok = false;
    m_eagle = Eagle::parseFile(ui->patheditLibrary->path(), &ok);
    if (!ok) {
        QMessageBox::critical(this,
                              tr("Unable to load library"),
                              tr("Unable to load the %1 library").arg(ui->patheditLibrary->path()));
    }
    return ok;
}
