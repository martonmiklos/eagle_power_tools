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

bool StencilLaserCutSVGExport::generateSVG(EagleLayers::PCBLayers layer,
                                           Mirror mirror,
                                           Schematic *sch, Board *brd,
                                           const QString &selectedVariant,
                                           const QString &svgPath)
{
    QSvgGenerator generator;
    generator.setFileName(svgPath);
    generator.setDescription(QObject::tr("Stencil SVG file generated from the %1 variant").arg(selectedVariant));
    generator.setTitle(QObject::tr("Stencil SVG file generated from the %1 variant").arg(selectedVariant));
    // painter paints in 1/32000 mm units
    generator.setResolution(812800);

    QRectF boardRect = EAGLE_Utils::boardBoundingRect(brd);
    generator.setViewBox(boardRect);
    generator.setSize(QSize(UnitUtilities::mmToU(boardRect.width()),
                      UnitUtilities::mmToU(boardRect.height())));

    QPainter painter(&generator);
    if (mirror == Generate_Mirrored) {
        painter.scale(1, -1);
        painter.translate(0, -boardRect.height());
    }
    painter.setBrush(QBrush(Qt::black));
    painter.setPen(QPen(Qt::black, 0));
    for (Element *element : *brd->elements()->elementList()) {
        bool add = true;        
        if (!selectedVariant.isEmpty()) {
            for (Part *part : *sch->parts()->partList()) {
                if (part->name() == element->name()) {
                    for (Variant *var : *part->variantList()) {
                        if (var->name() == selectedVariant) {
                            add = (var->populate() != Variant::Populate_no);
                            break;
                        }
                    }
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
                            qreal rotation = static_cast<qreal>(EAGLE_Utils::rotationToDegrees(element->rot(), &mirror));
                            if (mirror)
                                exportLayer = EagleLayers::oppositeLayer(exportLayer);
                            pkgFound = true;

                            painter.translate(element->x(), element->y());
                            painter.rotate(rotation);
                            drawPads(exportLayer, &painter, pkg);
                            drawPolygons(exportLayer, &painter, pkg);
                            drawRects(exportLayer, &painter, pkg);
                            // FIXME check circles, wires, in the lib too

                            painter.rotate(-rotation);
                            painter.translate(-element->x(), -element->y());
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

    for (Rectangle *rect : *brd->plain()->rectangleList()) {
        EagleLayers::PCBLayers exportLayer = layer;
        bool mirror = false;
        if (mirror)
            exportLayer = EagleLayers::oppositeLayer(exportLayer);
        if (rect->layer() == layer) {
            painter.drawRect(QRectF(rect->x1(),
                                      rect->y1(),
                                      rect->x2() - rect->x1(),
                                      rect->y2() - rect->y1()));
        }
    }

    for (Circle *circle : *brd->plain()->circleList()) {
        if (circle->layer() == layer) {
            painter.drawEllipse(QRectF(circle->x(), circle->y(), circle->width(), circle->width()));
        }
    }

    QPen oldPen = painter.pen();
    for (Wire *wire : *brd->plain()->wireList()) {
        if (wire->layer() == layer) {
            QPen pen = painter.pen();
            pen.setWidthF(wire->width());
            pen.setCapStyle(EAGLE_Utils::wireCapStyleToPenCapStyle(wire->cap()));
            painter.setPen(pen);
            painter.drawLine(QLineF(wire->x1(), wire->y1(), wire->x2(), wire->y2()));
        }
    }
    painter.setPen(oldPen);

    return true;
}


void StencilLaserCutSVGExport::drawPads(const EagleLayers::PCBLayers layer,
                                        QPainter *painter,
                                        Package *package)
{
    EagleLayers::PCBLayers activeLayer = layer == EagleLayers::tCream ? EagleLayers::Top : EagleLayers::Bottom;
    EagleLayers::PCBLayers oppositeLayer = EagleLayers::oppositeLayer(activeLayer);
    for (Smd* smd : *package->smdList()) {
        if ((!smd->rot().contains('M') && smd->layer() == activeLayer)
                || (smd->rot().contains('M') && smd->layer() == oppositeLayer)) {
            SmdPainter::drawCream(painter, smd);
        }
    }
}

void StencilLaserCutSVGExport::drawPolygons(const EagleLayers::PCBLayers layer,
                                            QPainter *painter,
                                            Package *package)
{
    for (Polygon *polygon : *package->polygonList()) {
        if (polygon->layer() == layer) {
            QPainterPath pp;
            for (Vertex *v : *polygon->vertexList()) {
                if (pp.isEmpty()) {
                    pp.moveTo(v->x(), v->y());
                } else {
                    if (v->curve() == 0.0) {
                        pp.lineTo(v->x(), v->y());
                    } else {
                        //pp.arcTo();
                        // FIXME
                    }
                }
            }
            painter->drawPath(pp);
        }
    }
}

void StencilLaserCutSVGExport::drawRects(const EagleLayers::PCBLayers layer,
                                         QPainter *painter,
                                         Package *package)
{
    for (Rectangle *rect : *package->rectangleList()) {
        if (rect->layer() == layer) {
            painter->drawRect(QRectF(rect->x1(),
                                      rect->y1(),
                                      rect->x2() - rect->x1(),
                                      rect->y2() - rect->y1()));
        }
    }
}
