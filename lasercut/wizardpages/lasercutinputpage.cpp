#include "lasercutinputpage.h"
#include "ui_lasercutinputpage.h"

LaserCutInputpage::LaserCutInputpage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LaserCutInputpage)
{
    ui->setupUi(this);
}

LaserCutInputpage::~LaserCutInputpage()
{
    delete ui;
}
