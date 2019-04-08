#ifndef STENCILLASERCUTSVGEXPORT_H
#define STENCILLASERCUTSVGEXPORT_H

#include "qt_eagle_xml_parser/eagle.h"

#include <QString>

class StencilLaserCutSVGExport
{
public:
    StencilLaserCutSVGExport();

    bool generateSVG(Board *brd, const QString& variant, const QString& svgPath);
private:
    void paintPads(QPainter* painter, Package *package);
};

#endif // STENCILLASERCUTSVGEXPORT_H
