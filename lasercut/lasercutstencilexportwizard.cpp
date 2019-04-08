#include "lasercutstencilexportwizard.h"

#include "stencillasercutsvgexport.h"

#include <QDebug>

LaserCutStencilExportWizard::LaserCutStencilExportWizard(QWidget *parent) :
    QWizard(parent)
{
    m_introPage = new LaserCutIntroPage(this);
    addPage(m_introPage);
    m_inputPage = new LaserCutInputpage(&m_settings, this);
    addPage(m_inputPage);
    m_outputPage = new LaserCutOutputPage(&m_settings, this);
    addPage(m_outputPage);
    m_finishPage = new LaserCutFinishPage(this);
    lastPage= addPage(m_finishPage);
    connect(this, &LaserCutStencilExportWizard::currentIdChanged,
            this, &LaserCutStencilExportWizard::currentIdChangedSlot);
}

void LaserCutStencilExportWizard::currentIdChangedSlot()
{
    qWarning() << currentId();

    if (currentId() == lastPage) {
        StencilLaserCutSVGExport exporter;
        if (m_outputPage->generateTop())
            exporter.generateSVG(EagleLayers::tCream, m_inputPage->brd()->drawing()->board(), m_inputPage->selectedVariant(), m_outputPage->topPath());
        if (m_outputPage->generateBottom())
            exporter.generateSVG(EagleLayers::bCream, m_inputPage->brd()->drawing()->board(), m_inputPage->selectedVariant(), m_outputPage->bottomPath());
    }
}
