#include "dialogsettings.h"
#include "ui_dialogsettings.h"

#include <QDir>

DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);
}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::showEvent(QShowEvent *event)
{
    ui->comboBoxInstalledEAGLEVersions->clear();

#ifdef Q_OS_LINUX
    // new EAGLE stores eaglerc under:
    // $HOME/.local/share/Eagle/settings/<version>
    QDir eagleSettingsDir(QDir::homePath() + QDir::separator() + ".local/share/Eagle/settings");
    for (QString settingsDir : eagleSettingsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString settingsFilePath = eagleSettingsDir.absolutePath() + QDir::separator() + settingsDir + QDir::separator() + "eaglerc";
        if (QFile::exists(settingsFilePath)) {
            ui->comboBoxInstalledEAGLEVersions->addItem(
                        settingsDir,
                        settingsFilePath
                        );
        }
    }

#endif
    ui->comboBoxInstalledEAGLEVersions->setCurrentText(m_settings.value("EAGLEConfigVersion").toString());
    QDialog::showEvent(event);
}

void DialogSettings::on_DialogSettings_accepted()
{
    m_settings.setValue("EAGLEConfigVersion", ui->comboBoxInstalledEAGLEVersions->currentText());
}
