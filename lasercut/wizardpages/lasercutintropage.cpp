#include "lasercutintropage.h"
#include "ui_lasercutintropage.h"

LaserCutIntroPage::LaserCutIntroPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LaserCutIntroPage)
{
    ui->setupUi(this);
}

LaserCutIntroPage::~LaserCutIntroPage()
{
    delete ui;
}
