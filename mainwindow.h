#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qmainwindow_with_recent_list/mainwindowwithrecentlist.h"

#include <QMainWindow>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public MainWindowWithRecentList
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionLoad_EAGLE_design_triggered();
    void on_actionSilkscreen_slicer_triggered();

    bool openFile(const QString & fileName) override;

private:
    Ui::MainWindow *ui;
    QSettings m_settings;
};

#endif // MAINWINDOW_H
