#ifndef LASERCUTINPUTPAGE_H
#define LASERCUTINPUTPAGE_H

#include <QSettings>
#include <QWizardPage>

#include "qt_eagle_xml_parser/eagle.h"

namespace Ui {
class LaserCutInputpage;
}

class LaserCutInputpage : public QWizardPage
{
    Q_OBJECT

public:
    explicit LaserCutInputpage(QSettings *settings, QWidget *parent = nullptr);
    ~LaserCutInputpage();

    QString selectedVariant() const;
    Eagle *brd() const;

private slots:
    void on_patheditBrdFile_pathChanged(const QString &path);

private:
    Ui::LaserCutInputpage *ui;
    QSettings *m_settings;
    Eagle *m_brd = nullptr;
};

#endif // LASERCUTINPUTPAGE_H
