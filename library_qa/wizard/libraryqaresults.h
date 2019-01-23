#ifndef LIBRARYQARESULTS_H
#define LIBRARYQARESULTS_H

#include <QWizardPage>

namespace Ui {
class LibraryQAResults;
}

class LibraryQAResults : public QWizardPage
{
    Q_OBJECT

public:
    explicit LibraryQAResults(QWidget *parent = nullptr);
    ~LibraryQAResults();

private:
    Ui::LibraryQAResults *ui;
};

#endif // LIBRARYQARESULTS_H
