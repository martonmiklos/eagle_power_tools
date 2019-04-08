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

bool StencilLaserCutSVGExport::generateSVG(Board *brd, const QString &variant, const QString &svgPath)
{
    QSvgGenerator generator;
    generator.setFileName(svgPath);
    generator.setResolution(1000); // set 1000 DPI

    QRectF boardRect = EAGLE_Utils::boardBoundingRect(brd);
    generator.setSize(QSize(UnitUtilities::mmToU(boardRect.width()),
                      UnitUtilities::mmToU(boardRect.height())));

    QPainter painter(&generator);
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
                            pkgFound = true;
                            paintPads(&painter, pkg);
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

void StencilLaserCutSVGExport::paintPads(QPainter *painter, Package *package)
{
    for (Smd* smd : *package->smdList()) {
        SmdPainter::drawCream(painter, smd);
    }
}
