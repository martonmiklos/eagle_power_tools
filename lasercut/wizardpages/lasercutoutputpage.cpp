#include "lasercutoutputpage.h"
#include "ui_lasercutoutputpage.h"

#include <QFileInfo>

LaserCutOutputPage::LaserCutOutputPage(LaserCutInputpage *inputPage,
                                       QSettings *settings,
                                       QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::LaserCutOutputPage),
    m_settings(settings),
    m_inputPage(inputPage)
{
    ui->setupUi(this);
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

void LaserCutOutputPage::loadSettings()
{
    m_settings->beginGroup("laserCutExport");
    ui->checkBoxTopStencil->setChecked(m_settings->value("topStencilEnabled", true).toBool());
    ui->checkBoxBottomStencil->setChecked(m_settings->value("bottomStencilEnabled", true).toBool());
    ui->checkBoxMirrorTop->setChecked(m_settings->value("mirrorTop", true).toBool());
    ui->checkBoxMirrorBottom->setChecked(m_settings->value("mirrorBottom", false).toBool());
    m_settings->endGroup();

    QFileInfo fi(m_inputPage->schPath());
    ui->patheditTopStencil->setPath(
                m_settings->value("topStencil",
                                  QDir::homePath()
                                  + QDir::separator()
                                  + fi.completeBaseName()
                                  + "_top.svg").toString());
    fi.setFile(m_inputPage->brdPath());
    ui->patheditBottomStencil->setPath(
                m_settings->value("bottomStencil",
                                  QDir::homePath()
                                  + QDir::separator()
                                  + fi.completeBaseName()
                                  + "_bottom.svg").toString());
}

void LaserCutOutputPage::saveSettings()
{
    m_settings->beginGroup("laserCutExport");
    m_settings->setValue("topStencil", ui->patheditTopStencil->path());
    m_settings->setValue("bottomStencil", ui->patheditBottomStencil->path());
    m_settings->setValue("topStencilEnabled", ui->checkBoxTopStencil->isChecked());
    m_settings->setValue("bottomStencilEnabled", ui->checkBoxBottomStencil->isChecked());
    m_settings->setValue("mirrorTop", ui->checkBoxMirrorTop->isChecked());
    m_settings->setValue("mirrorBottom", ui->checkBoxMirrorBottom->isChecked());
    m_settings->endGroup();
}

bool LaserCutOutputPage::mirrorTopStencil() const
{
    return ui->checkBoxMirrorTop->isChecked();
}

bool LaserCutOutputPage::mirrorBottomStencil() const
{
    return ui->checkBoxMirrorBottom->isChecked();
}
