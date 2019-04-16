#ifndef LASERCUTOUTPUTPAGE_H
#define LASERCUTOUTPUTPAGE_H

#include "lasercutinputpage.h"

#include <QSettings>
#include <QWizardPage>

namespace Ui {
class LaserCutOutputPage;
}

class LaserCutOutputPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit LaserCutOutputPage(LaserCutInputpage *inputPage,
                                QSettings *settings,
                                QWidget *parent = nullptr);
    ~LaserCutOutputPage();

    bool generateTop() const;
    bool generateBottom() const;

    QString topPath() const;
    QString bottomPath() const;

    void loadSettings();
    void saveSettings();

    bool mirrorTopStencil() const;
    bool mirrorBottomStencil() const;

private:
    Ui::LaserCutOutputPage *ui;
    QSettings *m_settings;
    LaserCutInputpage *m_inputPage;
};

#endif // LASERCUTOUTPUTPAGE_H
