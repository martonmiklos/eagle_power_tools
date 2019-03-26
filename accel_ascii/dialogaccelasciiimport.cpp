#include "dialogaccelasciiimport.h"
#include "ui_dialogaccelasciiimport.h"

DialogAccelAsciiImport::DialogAccelAsciiImport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAccelAsciiImport)
{
    ui->setupUi(this);
    loadWidgets();
}

DialogAccelAsciiImport::~DialogAccelAsciiImport()
{
    saveWidgets();
    delete ui;
}

void DialogAccelAsciiImport::on_pushButtonConvert_clicked()
{

}
