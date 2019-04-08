#ifndef LASERCUTSTENCILEXPORTWIZARD_H
#define LASERCUTSTENCILEXPORTWIZARD_H

#include "wizardpages/lasercutinputpage.h"
#include "wizardpages/lasercutintropage.h"
#include "wizardpages/lasercutfinishpage.h"
#include "wizardpages/lasercutoutputpage.h"


#include "qt_eagle_xml_parser/eagle.h"

#include <QSettings>
#include <QWizard>

class LaserCutStencilExportWizard : public QWizard
{
public:
    LaserCutStencilExportWizard(QWidget *parent = nullptr);
private:
    QSettings m_settings;
    LaserCutIntroPage *m_introPage;
    LaserCutInputpage *m_inputPage;
    LaserCutOutputPage *m_outputPage;
    LaserCutFinishPage *m_finishPage;

    int lastPage = -1;

private slots:
    void currentIdChangedSlot();
};

#endif // LASERCUTSTENCILEXPORTWIZARD_H
