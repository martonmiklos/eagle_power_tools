#include "lasercutfinishpage.h"
#include "ui_lasercutfinishpage.h"

LaserCutFinishPage::LaserCutFinishPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LaserCutFinishPage)
{
    ui->setupUi(this);
}

LaserCutFinishPage::~LaserCutFinishPage()
{
    delete ui;
}
