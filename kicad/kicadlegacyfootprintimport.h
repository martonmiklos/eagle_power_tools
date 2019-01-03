#ifndef KICADLIBIMPORTER_H
#define KICADLIBIMPORTER_H

#include "kicadimportutils.h"

#include "mpc/mpc.h"
#include "../qt_eagle_xml_parser/eagle.h"

#include <QString>

/**
 * @brief The KicadLegacyFootprintLibImporter class
 */
class KicadLegacyFootprintLibImporter
{
public:
    KicadLegacyFootprintLibImporter();

    bool parseLibrary(const QString & libraryPath);
    Package *parseModuleAstToPackage(mpc_ast_t *module_ast, const char *name);
    void parseCirclesAstToPackage(mpc_ast_t *circles_ast, Package *package);
    void parseLinesAstToPackage(mpc_ast_t *lines_ast, Package *package);
    void parsePolygonsAstToPackage(mpc_ast_t *polygons_ast, Package *package);
    void parsePadsAstToPackage(mpc_ast_t *lines_ast, Package *package);

private:
    KicadUnitConverter m_unitConverter;
};

#endif // KICADLIBIMPORTER_H
