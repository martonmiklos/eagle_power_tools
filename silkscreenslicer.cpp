#include "silkscreenslicer.h"

#include "qt_eagle_xml_parser/utils.h"

#include <QLabel>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <QTransform>

SilkscreenSlicer::SilkscreenSlicer(Eagle *design) :
    m_eagle(design)
{

}

void SilkscreenSlicer::checkSilkscreenWiresStopCollisions()
{
    int imageIndex = 0;
    for (Package pkg : *m_eagle->drawing()->library()->packages()->packageList()) {
        // loop on the tStop / Top layer
        for (Wire wire : *pkg.wireList()) {
            enum {
                WireLayerIndex,
                MaskIndex
            };
            int layerCrossCheck[2][2] = {
                {
                    EAGLE_Utils::layerIndexFromName("tPlace", m_eagle),
                    EAGLE_Utils::layerIndexFromName("Top", m_eagle)
                },
                {
                    EAGLE_Utils::layerIndexFromName("bPlace", m_eagle),
                    EAGLE_Utils::layerIndexFromName("Bottom", m_eagle)
                }
            };

            for (int i = 0; i<2; i++) {
                if (wire.layer() == layerCrossCheck[i][WireLayerIndex]) {
                    for (Smd smd : *pkg.smdList()) {
                        if (smd.layer() == layerCrossCheck[i][MaskIndex] && smd.stop() == Smd::Stop_yes) {
                            QPointF pt1, pt2, internalPt;
                            EAGLE_Utils::smdStopMaskWireIntersections(smd, wire, &internalPt,  &pt1, &pt2, 10);
                            return;
                        }
                    }
                }
            }
        }
    }
}

double SilkscreenSlicer::stopMaskPercentage() const
{
    return m_stopMaskPercentage;
}

void SilkscreenSlicer::setStopMaskPercentage(double stopMaskPercentage)
{
    m_stopMaskPercentage = stopMaskPercentage;
}
