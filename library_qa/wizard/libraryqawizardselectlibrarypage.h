#ifndef LIBRARYQAWIZARDSELECTLIBRARYPAGE_H
#define LIBRARYQAWIZARDSELECTLIBRARYPAGE_H

#include "eagle.h"

#include <QSettings>
#include <QWizardPage>

namespace Ui {
class LibraryQAWizardSelectLibraryPage;
}

class LibraryQAWizardSelectLibraryPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit LibraryQAWizardSelectLibraryPage(QWidget *parent = nullptr);
    ~LibraryQAWizardSelectLibraryPage() override;

    bool isComplete() const override;

    Eagle *eagle() const;
    void setEagle(Eagle *eagle);

    bool validatePage() override;

private slots:
    void on_patheditLibrary_pathChanged(const QString &path);

private:
    Ui::LibraryQAWizardSelectLibraryPage *ui;
    QSettings m_settings;

    Eagle *m_eagle = nullptr;
};

#endif // LIBRARYQAWIZARDSELECTLIBRARYPAGE_H
