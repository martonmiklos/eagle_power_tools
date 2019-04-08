#ifndef STENCILLASERCUTSVGEXPORT_H
#define STENCILLASERCUTSVGEXPORT_H

#include "qt_eagle_xml_parser/eagle.h"
#include "qt_eagle_xml_parser/eaglelayers.h"

#include <QString>

class StencilLaserCutSVGExport
{
public:
    StencilLaserCutSVGExport();

    bool generateSVG(EagleLayers::PCBLayers layer, Board *brd, const QString& variant, const QString& svgPath);
private:
    void paintPads(const QPointF pos, qreal rotation, EagleLayers::PCBLayers layer, QPainter* painter, Package *package);
};

#endif // STENCILLASERCUTSVGEXPORT_H
