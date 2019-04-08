#include "lasercutoutputpage.h"
#include "ui_lasercutoutputpage.h"

LaserCutOutputPage::LaserCutOutputPage(QSettings *settings, QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LaserCutOutputPage),
    m_settings(settings)
{
    ui->setupUi(this);

    m_settings->beginGroup("laserCutExport");
    ui->patheditTopStencil->setPath(
                m_settings->value("topStencil",
                                  QDir::homePath() + QDir::separator() + "top.svg").toString());
    ui->patheditBottomStencil->setPath(
                m_settings->value("bottomStencil",
                                  QDir::homePath() + QDir::separator() + "bottom.svg").toString());
    ui->checkBoxTopStencil->setChecked(m_settings->value("topStencilEnabled", true).toBool());
    ui->checkBoxBottomStencil->setChecked(m_settings->value("bottomStencilEnabled", true).toBool());
    m_settings->endGroup();
}

LaserCutOutputPage::~LaserCutOutputPage()
{
    delete ui;
}

bool LaserCutOutputPage::generateTop() const
{
    return  ui->checkBoxTopStencil->isChecked();
}

bool LaserCutOutputPage::generateBottom() const
{
    return  ui->checkBoxBottomStencil->isChecked();
}

QString LaserCutOutputPage::topPath() const
{
    return ui->patheditTopStencil->path();
}

QString LaserCutOutputPage::bottomPath() const
{
    return ui->patheditBottomStencil->path();
}

void LaserCutOutputPage::on_patheditTopStencil_pathChanged(const QString &path)
{
    m_settings->beginGroup("laserCutExport");
    m_settings->setValue("topStencil", path);
    m_settings->endGroup();
}

void LaserCutOutputPage::on_patheditBottomStencil_pathChanged(const QString &path)
{
    m_settings->beginGroup("laserCutExport");
    m_settings->setValue("bottomStencil", path);
    m_settings->endGroup();
}

void LaserCutOutputPage::on_checkBoxTopStencil_toggled(bool checked)
{
    m_settings->beginGroup("laserCutExport");
    m_settings->setValue("topStencilEnabled", checked);
    m_settings->endGroup();
}

void LaserCutOutputPage::on_checkBoxBottomStencil_toggled(bool checked)
{
    m_settings->beginGroup("laserCutExport");
    m_settings->setValue("bottomStencilEnabled", checked);
    m_settings->endGroup();
}
