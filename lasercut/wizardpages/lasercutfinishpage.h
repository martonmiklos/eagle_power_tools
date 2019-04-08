#ifndef LASERCUTFINISHPAGE_H
#define LASERCUTFINISHPAGE_H

#include <QWizardPage>

namespace Ui {
class LaserCutFinishPage;
}

class LaserCutFinishPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit LaserCutFinishPage(QWidget *parent = nullptr);
    ~LaserCutFinishPage();

private:
    Ui::LaserCutFinishPage *ui;
};

#endif // LASERCUTFINISHPAGE_H
