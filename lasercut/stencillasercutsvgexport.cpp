#include "stencillasercutsvgexport.h"

#include "qt_eagle_xml_parser/eagle.h"
#include "qt_eagle_xml_parser/smdpainter.h"
#include "qt_eagle_xml_parser/unitutilities.h"
#include "qt_eagle_xml_parser/utils.h"

#include <QDebug>
#include <QPainter>
#include <QtSvg/QSvgGenerator>

StencilLaserCutSVGExport::StencilLaserCutSVGExport()
{

}

bool StencilLaserCutSVGExport::generateSVG(EagleLayers::PCBLayers layer, Board *brd, const QString &variant, const QString &svgPath)
{
    QSvgGenerator generator;
    generator.setFileName(svgPath);
    generator.setDescription(QObject::tr("Stencil SVG file generated from the %1 variant").arg(variant));
    generator.setTitle(QObject::tr("Stencil SVG file generated from the %1 variant").arg(variant));
    // painter paints in 1/32000 mm units
    generator.setResolution(812800);

    QRectF boardRect = EAGLE_Utils::boardBoundingRect(brd);
    generator.setViewBox(boardRect);
    generator.setSize(QSize(UnitUtilities::mmToU(boardRect.width()),
                      UnitUtilities::mmToU(boardRect.height())));

    QPainter painter(&generator);
    painter.setBrush(QBrush(Qt::black));
    painter.setPen(QPen(Qt::black, 0));
    for (Element *element : *brd->elements()->elementList()) {
        bool add = true;        
        if (!variant.isEmpty()) {
            for (Variant *v : *element->variantList()) {
                if (v->name() == variant) {
                    add = (v->populate() != Variant::Populate_no);
                    break;
                }
            }
        }

        if (add) {
            bool pkgFound = false;
            for (Library *lib : *brd->libraries()->libraryList()) {
                if (lib->name() == element->library()) {
                    for (Package* pkg : *lib->packages()->packageList()) {
                        if (pkg->name() == element->package()) {
                            EagleLayers::PCBLayers exportLayer = layer;
                            bool mirror = false;
                            qreal rotation = EAGLE_Utils::rotationToDegrees(element->rot(), &mirror);
                            if (mirror)
                                exportLayer = EagleLayers::oppositeLayer(exportLayer);
                            pkgFound = true;
                            paintPads(QPointF(element->x(), element->y()), rotation, exportLayer, &painter, pkg);
                            break;
                        }
                    }
                    break;
                }
            }
            if (!pkgFound) {
                qWarning() << "Package " << element->package() << " not found in the libraries section";
            }
        }
    }
    return true;
}

void StencilLaserCutSVGExport::paintPads(const QPointF pos, qreal rotation, EagleLayers::PCBLayers layer, QPainter *painter, Package *package)
{
    EagleLayers::PCBLayers activeLayer = layer == EagleLayers::tCream ? EagleLayers::Top : EagleLayers::Bottom;
    EagleLayers::PCBLayers oppositeLayer = EagleLayers::oppositeLayer(activeLayer);
    painter->translate(pos.x(), pos.y());
    painter->rotate(rotation);
    for (Smd* smd : *package->smdList()) {
        if ((!smd->rot().contains('M') && smd->layer() == activeLayer)
                || (smd->rot().contains('M') && smd->layer() == oppositeLayer)) {
            SmdPainter::drawCream(painter, smd);
        }
    }
    painter->rotate(-rotation);
    painter->translate(-pos.x(), -pos.y());
}
