#include "silkscreenslicer.h"

#include "qt_eagle_xml_parser/utils.h"

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
                            // rounded corner SMD pads are more difficult
                            QPainterPath rectanglePath;
                            rectanglePath.addRoundedRect(EAGLE_Utils::smdToQRectF(smd), smd.roundness(), smd.roundness());

                            qreal rotation = smd.rot().mid(1).toInt();
                            QTransform transform;
                            transform.rotate(rotation);
                            transform.translate(smd.dx(), smd.dy());

                            rectanglePath = transform.map(rectanglePath);

                            if (rectanglePath.contains(QPointF(wire.x1(), wire.y1())) ||
                                    rectanglePath.contains(QPointF(wire.x2(), wire.y2()))) {
                                // stop mask and wire collides
                                //qWarning() <<
                            }
                        }
                    }
                }
            }
        }
    }
}
