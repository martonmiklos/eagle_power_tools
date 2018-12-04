#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graphicsutilities.h"
#include "silkscreenslicer.h"
#include "qt_eagle_xml_parser/utils.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    MainWindowWithRecentList(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadRecentFiles(ui->menuRecent_designs);
    //connect(ui->ta)
    Eagle design = Eagle::parseFile("/opt/eagle/lbr/test2.lbr", nullptr);
    SilkscreenSlicer slicer = SilkscreenSlicer(&design);
    slicer.checkSilkscreenWiresStopCollisions();

    m_settingsDialog = new DialogSettings(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    MainWindowWithRecentList::closeEvent(event);
    QCoreApplication::exit();
}

bool MainWindow::openFile(const QString &fileName)
{
    return false;
}

void MainWindow::on_actionLoad_EAGLE_design_triggered()
{
    QString dirName = QFileDialog::getExistingDirectory(this,
                                                        tr("Select EAGLE design directory"),
                                                        m_settings.value("lastDesignDir", QDir::homePath()).toString());
    if (!dirName.isEmpty()) {
        QStringList designs;
        //for ( QDir(dirName).entryList("*.sch"));
    }
}

void MainWindow::on_actionSilkscreen_slicer_triggered()
{
    QString lastSlicedLbr = QFileDialog::getOpenFileName(this,
                                                        tr("Select EAGLE design directory"),
                                                        m_settings.value("lastSlicedLbr", QDir::homePath()).toString(),
                                                        tr("EAGLE libraries (*.lbr)"));
    if (!lastSlicedLbr.isEmpty()) {
        bool ok = false;
        Eagle design = Eagle::parseFile(lastSlicedLbr, &ok);
        if (ok) {
            SilkscreenSlicer slicer = SilkscreenSlicer(&design);
            slicer.checkSilkscreenWiresStopCollisions();
            m_settings.setValue("lastSlicedLbr", lastSlicedLbr);
        }
    }
}

void MainWindow::on_actionSettings_triggered()
{
    m_settingsDialog->show();
}

void MainWindow::on_actionLibrary_QA_triggered()
{

}
