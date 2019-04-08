#ifndef LASERCUTOUTPUTPAGE_H
#define LASERCUTOUTPUTPAGE_H

#include <QSettings>
#include <QWizardPage>

namespace Ui {
class LaserCutOutputPage;
}

class LaserCutOutputPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit LaserCutOutputPage(QSettings *settings, QWidget *parent = nullptr);
    ~LaserCutOutputPage();

    bool generateTop() const;
    bool generateBottom() const;

    QString topPath() const;
    QString bottomPath() const;

private slots:
    void on_patheditTopStencil_pathChanged(const QString &path);
    void on_patheditBottomStencil_pathChanged(const QString &path);
    void on_checkBoxTopStencil_toggled(bool checked);
    void on_checkBoxBottomStencil_toggled(bool checked);

private:
    Ui::LaserCutOutputPage *ui;
    QSettings *m_settings;
};

#endif // LASERCUTOUTPUTPAGE_H
