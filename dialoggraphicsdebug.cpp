#include "dialoggraphicsdebug.h"
#include "ui_dialoggraphicsdebug.h"

DialogGraphicsDebug::DialogGraphicsDebug(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGraphicsDebug)
{
    ui->setupUi(this);

}

DialogGraphicsDebug::~DialogGraphicsDebug()
{
    delete ui;
}

QPainter *DialogGraphicsDebug::painter() const
{
    return m_painter;
}
