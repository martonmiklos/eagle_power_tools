#ifndef DIALOGSETTINGS_H
#define DIALOGSETTINGS_H

#include <QDialog>
#include <QShowEvent>
#include <QSettings>

namespace Ui {
class DialogSettings;
}

class DialogSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSettings(QWidget *parent = 0);
    ~DialogSettings();

    void showEvent(QShowEvent *event);

private slots:
    void on_DialogSettings_accepted();

private:
    Ui::DialogSettings *ui;
    QSettings m_settings;
};

#endif // DIALOGSETTINGS_H
