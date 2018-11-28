#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

#include "silkscreenslicer.h"

MainWindow::MainWindow(QWidget *parent) :
    MainWindowWithRecentList(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadRecentFiles(ui->menuRecent_designs);
    //connect(ui->ta)
}

MainWindow::~MainWindow()
{
    delete ui;
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
