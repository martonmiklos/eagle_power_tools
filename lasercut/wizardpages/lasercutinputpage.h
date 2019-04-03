#ifndef LASERCUTINPUTPAGE_H
#define LASERCUTINPUTPAGE_H

#include <QWizardPage>

namespace Ui {
class LaserCutInputpage;
}

class LaserCutInputpage : public QWizardPage
{
    Q_OBJECT

public:
    explicit LaserCutInputpage(QWidget *parent = 0);
    ~LaserCutInputpage();

private:
    Ui::LaserCutInputpage *ui;
};

#endif // LASERCUTINPUTPAGE_H
