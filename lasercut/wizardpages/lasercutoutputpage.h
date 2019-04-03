#ifndef LASERCUTOUTPUTPAGE_H
#define LASERCUTOUTPUTPAGE_H

#include <QWizardPage>

namespace Ui {
class LaserCutOutputPage;
}

class LaserCutOutputPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit LaserCutOutputPage(QWidget *parent = 0);
    ~LaserCutOutputPage();

private:
    Ui::LaserCutOutputPage *ui;
};

#endif // LASERCUTOUTPUTPAGE_H
