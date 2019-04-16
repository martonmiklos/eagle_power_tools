#ifndef STENCILLASERCUTSVGEXPORT_H
#define STENCILLASERCUTSVGEXPORT_H

#include "qt_eagle_xml_parser/eagle.h"
#include "qt_eagle_xml_parser/eaglelayers.h"

#include <QString>

class StencilLaserCutSVGExport
{
public:
    StencilLaserCutSVGExport();
    enum Mirror {
        Generate_NonMirrored,
        Generate_Mirrored
    };
    bool generateSVG(EagleLayers::PCBLayers layer,
                     Mirror mirror,
                     Schematic *sch,
                     Board *brd,
                     const QString& boardVariant,
                     const QString& svgPath);

private:
    void drawPads(const EagleLayers::PCBLayers layer,
                   QPainter* painter,
                   Package *package);
    void drawPolygons(const EagleLayers::PCBLayers layer,
                       QPainter* painter,
                       Package* package);
    void drawRects(const EagleLayers::PCBLayers layer,
                       QPainter* painter,
                       Package* package);
};

#endif // STENCILLASERCUTSVGEXPORT_H
