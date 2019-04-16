#include "lasercutinputpage.h"
#include "ui_lasercutinputpage.h"


#include <QDebug>
#include <QMessageBox>

LaserCutInputpage::LaserCutInputpage(QSettings *settings, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LaserCutInputpage),
    m_settings(settings)
{
    ui->setupUi(this);
}

LaserCutInputpage::~LaserCutInputpage()
{
    delete ui;
}

void LaserCutInputpage::loadSettings()
{
    m_settings->beginGroup("laserCutExport");
    ui->patheditBrdFile->setPath(m_settings->value("lastBrdFile", QDir::homePath()).toString());
    m_settings->endGroup();
}

QString LaserCutInputpage::selectedVariant() const
{
    return ui->comboBoxBoardVariant->currentText();
}

QString LaserCutInputpage::schPath() const
{
    return ui->patheditSchematicFile->path();
}

QString LaserCutInputpage::brdPath() const
{
    return ui->patheditBrdFile->path();
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

    QFileInfo fi(path);
    qWarning() << fi.dir().path() + QDir::separator() + fi.completeBaseName() + ".sch";
    QString schPath = fi.dir().path() + QDir::separator() + fi.completeBaseName() + ".sch";
    if (QFile::exists(schPath)) {
        ui->patheditSchematicFile->setPath(schPath);
        for (Variantdef *var : *m_brd->drawing()->board()->variantdefs()->variantdefList()) {
            ui->comboBoxBoardVariant->addItem(var->name());
        }
        ui->comboBoxBoardVariant->setVisible(true);
    } else {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("The sch pair of the loaded brd file cannot be found.\n"
                                "Generating stencil for different board variants is not possibe."));
        ui->comboBoxBoardVariant->setVisible(false);
    }
}

Eagle *LaserCutInputpage::brd() const
{
    return m_brd;
}


void LaserCutInputpage::on_patheditSchematicFile_pathChanged(const QString &path)
{
    if (m_sch)
        delete m_sch;
    bool ok = false;
    m_sch = Eagle::parseFile(path, &ok);

    QFileInfo fi(path);
    QString schPath = fi.dir().path() + QDir::separator() + fi.baseName() + ".sch";
    if (!ok) {
        QMessageBox::warning(this,
                             tr("Warning"),
                             tr("Unable to load the sch file.\n"
                                "Generating stencil for different board variants is not possibe."));
        ui->comboBoxBoardVariant->setVisible(false);
    } else {
        ui->comboBoxBoardVariant->setVisible(true);
    }
}

Eagle *LaserCutInputpage::sch() const
{
    return m_sch;
}

