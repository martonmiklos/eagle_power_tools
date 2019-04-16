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

    void loadSettings();

    QString selectedVariant() const;
    QString schPath() const;
    QString brdPath() const;
    Eagle *brd() const;
    Eagle *sch() const;

private slots:
    void on_patheditBrdFile_pathChanged(const QString &path);
    void on_patheditSchematicFile_pathChanged(const QString &path);

private:
    Ui::LaserCutInputpage *ui;
    QSettings *m_settings;
    Eagle *m_brd = nullptr;
    Eagle *m_sch = nullptr;
};

#endif // LASERCUTINPUTPAGE_H
