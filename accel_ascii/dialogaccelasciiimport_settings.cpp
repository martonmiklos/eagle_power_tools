#include <QSettings>

#include "dialogaccelasciiimport.h"
#include "ui_dialogaccelasciiimport.h"


void DialogAccelAsciiImport::saveWidgets() const
{
    QSettings settings;
    settings.setValue("DialogAccelAsciiImport", this->saveGeometry());
    settings.setValue("patheditPCBFile", ui->patheditPCBFile->saveGeometry());
    settings.setValue("patheditSch2", ui->patheditSch2->saveGeometry());
    settings.setValue("patheditSch1", ui->patheditSch1->saveGeometry());
}

void DialogAccelAsciiImport::loadWidgets()
{
    QSettings settings;
    this->restoreGeometry(settings.value("DialogAccelAsciiImport").toByteArray());
    ui->patheditPCBFile->restoreGeometry(settings.value("patheditPCBFile").toByteArray());
    ui->patheditSch2->restoreGeometry(settings.value("patheditSch2").toByteArray());
    ui->patheditSch1->restoreGeometry(settings.value("patheditSch1").toByteArray());
}
