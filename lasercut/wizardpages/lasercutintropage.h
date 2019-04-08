#ifndef LASERCUTINTROPAGE_H
#define LASERCUTINTROPAGE_H

#include <QWizardPage>

namespace Ui {
class LaserCutIntroPage;
}

class LaserCutIntroPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit LaserCutIntroPage(QWidget *parent = nullptr);
    ~LaserCutIntroPage();

private:
    Ui::LaserCutIntroPage *ui;
};

#endif // LASERCUTINTROPAGE_H
