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
    m_outputPage = new LaserCutOutputPage(m_inputPage, &m_settings, this);
    addPage(m_outputPage);
    m_finishPage = new LaserCutFinishPage(this);
    addPage(m_finishPage);
    connect(this, &LaserCutStencilExportWizard::currentIdChanged,
            this, &LaserCutStencilExportWizard::currentIdChangedSlot);
}

void LaserCutStencilExportWizard::currentIdChangedSlot()
{
    qWarning() << currentId();

    switch (static_cast<PageId>(currentId())) {
    case LaserCutStencilExportWizard::PageId_Intro:
        break;
    case LaserCutStencilExportWizard::PageId_Input:
        m_inputPage->loadSettings();
        break;
    case LaserCutStencilExportWizard::PageId_Output:
        m_outputPage->loadSettings();
        break;
    case LaserCutStencilExportWizard::PageId_Finish: {
        m_outputPage->saveSettings();
        StencilLaserCutSVGExport exporter;
        if (m_outputPage->generateTop()) {
            exporter.generateSVG(EagleLayers::tCream,
                                 m_outputPage->mirrorTopStencil()
                                 ? StencilLaserCutSVGExport::Generate_Mirrored
                                 : StencilLaserCutSVGExport::Generate_NonMirrored,
                                 m_inputPage->sch()->drawing()->schematic(),
                                 m_inputPage->brd()->drawing()->board(),
                                 m_inputPage->selectedVariant(),
                                 m_outputPage->topPath());
        }

        if (m_outputPage->generateBottom()) {
            exporter.generateSVG(EagleLayers::bCream,
                                 m_outputPage->mirrorBottomStencil()
                                 ? StencilLaserCutSVGExport::Generate_Mirrored
                                 : StencilLaserCutSVGExport::Generate_NonMirrored,
                                 m_inputPage->sch()->drawing()->schematic(),
                                 m_inputPage->brd()->drawing()->board(),
                                 m_inputPage->selectedVariant(),
                                 m_outputPage->bottomPath());
        }
    } break;
    }
}
