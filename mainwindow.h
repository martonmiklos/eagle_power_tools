#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qmainwindow_with_recent_list/mainwindowwithrecentlist.h"

#include "accel_ascii/dialogaccelasciiimport.h"
#include "dialogsettings.h"
#include "druloader.h"
#include "lasercut/lasercutstencilexportwizard.h"
#include "library_qa/wizard/libraryqawizard.h"

#include <QCloseEvent>
#include <QMainWindow>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public MainWindowWithRecentList
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionLoad_EAGLE_design_triggered();
    void on_actionSilkscreen_slicer_triggered();
    bool openFile(const QString & fileName) override;
    void on_actionSettings_triggered();
    void on_actionLibrary_QA_triggered();
    void on_actionGerber_import_fix_triggered();
    void on_actionConvert_Kicad_library_triggered();
    void on_actionImport_Accel_ascii_triggered();
    void on_actionGenerate_lasercut_stencils_triggered();

private:
    Ui::MainWindow *ui;
    DialogSettings *m_settingsDialog;
    DialogAccelAsciiImport *m_accelImportDialog = nullptr;
    DRULoader *m_dru;
    LibraryQAWizard *m_libQAWizard = nullptr;
    LaserCutStencilExportWizard *m_laserCutWizard = nullptr;
};

#endif // MAINWINDOW_H
