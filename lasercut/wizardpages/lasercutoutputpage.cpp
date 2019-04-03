#include "lasercutoutputpage.h"
#include "ui_lasercutoutputpage.h"

LaserCutOutputPage::LaserCutOutputPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LaserCutOutputPage)
{
    ui->setupUi(this);
}

LaserCutOutputPage::~LaserCutOutputPage()
{
    delete ui;
}
