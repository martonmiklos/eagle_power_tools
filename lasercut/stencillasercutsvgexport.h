#ifndef STENCILLASERCUTSVGEXPORT_H
#define STENCILLASERCUTSVGEXPORT_H

#include <QString>

class StencilLaserCutSVGExport
{
public:
    StencilLaserCutSVGExport();

    bool generateSVG(const QString& brdPath, const QString& svgPath);
};

#endif // STENCILLASERCUTSVGEXPORT_H
