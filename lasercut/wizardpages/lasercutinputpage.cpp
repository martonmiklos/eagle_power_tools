#include "lasercutinputpage.h"
#include "ui_lasercutinputpage.h"

LaserCutInputpage::LaserCutInputpage(QSettings *settings, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LaserCutInputpage),
    m_settings(settings)
{
    ui->setupUi(this);

    m_settings->beginGroup("laserCutExport");
    ui->patheditBrdFile->setPath(m_settings->value("lastBrdFile", QDir::homePath()).toString());
    m_settings->endGroup();
}

LaserCutInputpage::~LaserCutInputpage()
{
    delete ui;
}

QString LaserCutInputpage::selectedVariant() const
{
    return ui->comboBoxBoardVariant->currentText();
}

void LaserCutInputpage::on_patheditBrdFile_pathChanged(const QString &path)
{
    m_settings->beginGroup("laserCutExport");
    m_settings->setValue("lastBrdFile", path);
    m_settings->endGroup();

    if (m_brd)
        delete m_brd;
    bool ok = false;
    m_brd = Eagle::parseFile(path, &ok);
    ui->comboBoxBoardVariant->clear();
    for (Variantdef *var : *m_brd->drawing()->board()->variantdefs()->variantdefList()) {
        ui->comboBoxBoardVariant->addItem(var->name());
    }
}

Eagle *LaserCutInputpage::brd() const
{
    return m_brd;
}

