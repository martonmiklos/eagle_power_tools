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

    Library *parseModFile(const QString & libraryPath);
    void save();

private:
    mpc_ast_t *parseModfileToAst(const char *file_name, const char *string);
    KicadUnitConverter m_unitConverter;

    Package *parseModuleAstToPackage(mpc_ast_t *module_ast, const char *name);
    void parseCirclesAstToPackage(mpc_ast_t *circles_ast, Package *package);
    void parseLinesAstToPackage(mpc_ast_t *lines_ast, Package *package);
    void parsePolygonsAstToPackage(mpc_ast_t *polygons_ast, Package *package);
    void parsePadsAstToPackage(mpc_ast_t *lines_ast, Package *package);

    QString m_libPath;
    Eagle *m_eagle;
};

#endif // KICADLIBIMPORTER_H
