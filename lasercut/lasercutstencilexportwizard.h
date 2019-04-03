#ifndef LASERCUTSTENCILEXPORTWIZARD_H
#define LASERCUTSTENCILEXPORTWIZARD_H

#include "wizardpages/lasercutintropage.h"
#include "wizardpages/lasercutoutputpage.h"
#include "wizardpages/lasercutinputpage.h"

#include <QWizard>

class LaserCutStencilExportWizard : public QWizard
{
public:
    LaserCutStencilExportWizard(QWidget *parent = nullptr);
private:
    LaserCutIntroPage *m_introPage;
    LaserCutInputpage *m_inputPage;
    LaserCutOutputPage *m_outputPage;
};

#endif // LASERCUTSTENCILEXPORTWIZARD_H
