#include "silkscreenslicer.h"

#include "qt_eagle_xml_parser/utils.h"

#include <QDebug>
#include <QPainterPath>
#include <QTransform>

SilkscreenSlicer::SilkscreenSlicer(Eagle *design) :
    m_eagle(design)
{

}

void SilkscreenSlicer::checkSilkscreenWiresStopCollisions()
{
    for (Package pkg : m_eagle->drawing().library().packages().packageList()) {
        // loop on the tStop / Top layer
        for (Wire wire : pkg.wireList()) {
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
                    for (Smd smd : pkg.smdList()) {
                        if (smd.layer() == layerCrossCheck[i][MaskIndex] && smd.stop() == Smd::Stop_yes) {
                            QPainterPath rectanglePath, linePath;
                            rectanglePath.addRoundedRect(EAGLE_Utils::smdToQRectF(smd),
                                                         smd.roundness(),
                                                         smd.roundness(),
                                                         Qt::RelativeSize);

                            qreal rotation = smd.rot().mid(1).toInt();
                            /*QTransform transform;
                            transform.translate(smd.dx(), smd.dy());
                            transform.rotate(rotation);*/
                            qWarning() << rectanglePath;

                            linePath.moveTo(wire.x1(), wire.y1());
                            linePath.lineTo(wire.x2(), wire.y2());
                            qWarning() << linePath;
                            if (!rectanglePath.intersected(linePath).isEmpty()) {
                                qWarning() << "Collosion on stop layers";
                            }
                        }
                    }
                }
            }
        }
    }
}
