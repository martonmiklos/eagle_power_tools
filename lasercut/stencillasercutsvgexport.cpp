#include "stencillasercutsvgexport.h"

#include "qt_eagle_xml_parser/eagle.h"

StencilLaserCutSVGExport::StencilLaserCutSVGExport()
{

}

bool StencilLaserCutSVGExport::generateSVG(const QString &brdPath, const QString &svgPath)
{
    bool ok = false;
    Eagle *brd = Eagle::parseFile(brdPath, &ok);
    if (ok) {

        return true;
    }
    return false;
}
