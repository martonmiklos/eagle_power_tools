#include "lasercutstencilexportwizard.h"

LaserCutStencilExportWizard::LaserCutStencilExportWizard(QWidget *parent) :
    QWizard(parent)
{
    m_introPage = new LaserCutIntroPage(this);
    addPage(m_introPage);
    m_inputPage = new LaserCutInputpage(this);
    addPage(m_inputPage);
    m_outputPage = new LaserCutOutputPage(this);
    addPage(m_outputPage);
}
