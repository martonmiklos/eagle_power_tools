#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "eagle_librarysaver.h"
#include "gerberimportfixer.h"
#include "graphicsutilities.h"
#include "kicad/kicadlegacyfootprintimport.h"
#include "silkscreenslicer.h"
#include "qt_eagle_xml_parser/utils.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    MainWindowWithRecentList(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    loadRecentFiles(ui->menuRecent_designs);

    KicadLegacyFootprintLibImporter importer;
    importer.parseModFile("/tmp/AA01B-S040VA1.mod");

    m_dru = new DRULoader("/opt/eagle/dru/default_test.dru");

    /*Eagle *design = Eagle::parseFile("/opt/eagle/lbr/test2.lbr", nullptr);
    SilkscreenSlicer slicer(design, m_dru);
    slicer.fixSilkscreenWiresStopMaskCollisions();
    design->writeFile("/tmp/foo.lbr");*/


    //bool ok = false;
    //GerberImportFixer gf = GerberImportFixer("/home/mm/Projektek/eagle/integra_reveng_cad/DMSI41_single.brd");
    //GerberImportFixer gf = GerberImportFixer("/home/mm/Projektek/eagle/integra_reveng_cad/DMSI41 working gerber.brd");
    //GerberImportFixer gf = GerberImportFixer("/home/mm/Projektek/eagle/test/test.brd");
    //gf.fix();

    m_settingsDialog = new DialogSettings(this);
}

MainWindow::~MainWindow()
{
    delete m_dru;
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
        Eagle *design = Eagle::parseFile(lastSlicedLbr, &ok);
        if (ok) {
            SilkscreenSlicer slicer(design, m_dru);
            slicer.fixSilkscreenWiresStopMaskCollisions();
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
    if (m_libQAWizard == nullptr)
        m_libQAWizard = new LibraryQAWizard(this);
    m_libQAWizard->show();
}

void MainWindow::on_actionGerber_import_fix_triggered()
{
    QString lastGerberFix = QFileDialog::getOpenFileName(this,
                                                         tr("Select EAGLE design directory"),
                                                         m_settings.value("lastGerberFix", QDir::homePath()).toString(),
                                                         tr("EAGLE brd files (*.brd)"));
    if (!lastGerberFix.isEmpty()) {
        GerberImportFixer gf = GerberImportFixer(lastGerberFix);
        gf.fix();
        m_settings.setValue("lastGerberFix", lastGerberFix);
    }
}

void MainWindow::on_actionConvert_Kicad_library_triggered()
{
    m_settings.beginGroup("kicadconversion");
    QString kicadLibPath = QFileDialog::getOpenFileName(this,
                                                            tr("Select Kicad library to convert"),
                                                            m_settings.value("lastKicadLibConvertInputFile",
                                                                             QDir::homePath()).toString(),
                                                            tr("Kicad library files (*.mod)"));
    if (!kicadLibPath.isEmpty()) {
        QFileInfo fiInput(kicadLibPath);
        QString outPath = QFileDialog::getSaveFileName(
                    this,
                    tr("Save to EAGLE lbr"),
                    m_settings.value("lastKicadLibConvertOutdir", QDir::homePath()).toString() +
                    QDir::separator() +
                    fiInput.completeBaseName() + QStringLiteral(".lbr"),
                    tr("EAGLE library file (*.lbr)"));
        if (!outPath.isEmpty()) {
            KicadLegacyFootprintLibImporter importer;
            Library *lib = importer.parseModFile(kicadLibPath);
            if (EAGLE_LibrarySaver::saveLibrary(lib, outPath)) {
                QFileInfo fi(outPath);
                m_settings.setValue("lastKicadLibConvertOutdir", fi.path());
                m_settings.setValue("lastKicadLibConvertInputFile", kicadLibPath);

                QMessageBox::information(this,
                                         tr("Success"),
                                         tr("Library converted successfully!"));
            } else {
                QMessageBox::warning(this,
                                         tr("Error"),
                                         tr("Unable to convert the library!"));
            }
        }
    }
    m_settings.endGroup();
}

void MainWindow::on_actionImport_Accel_ascii_triggered()
{
    if (m_accelImportDialog == nullptr) {
        m_accelImportDialog = new DialogAccelAsciiImport(this);
    }
    m_accelImportDialog->show();
}

void MainWindow::on_actionGenerate_lasercut_stencils_triggered()
{
    if (m_laserCutWizard == nullptr)
        m_laserCutWizard = new LaserCutStencilExportWizard(this);
    m_laserCutWizard->restart();
    m_laserCutWizard->show();
    /*m_settings.beginGroup("laserCut");
    QString brdPath = QFileDialog::getOpenFileName(this,
                                                   tr("Select Brd file"),
                                                   m_settings.value("lastSoruceFile").toString(),
                                                   tr("EAGLE Brd files (*.brd)"));
    if (!brdPath.isEmpty()) {
        QString svgPath = QFileDialog::getSaveFileName(this,
                                                       tr("Select the SVG file to save"),
                                                       m_settings.value("lastOutputFile", QDir::homePath()).toString(),
                                                       tr("SVG files (*.svg)"));
        if (svgPath.isEmpty()) {
            m_settings.setValue("lastSoruceFile", brdPath);
            m_settings.setValue("lastOutputFile", svgPath);
        }
    }
    m_settings.endGroup();*/
}
