#ifndef KICADLEGACYSYMBOLFILEIMPORT_H
#define KICADLEGACYSYMBOLFILEIMPORT_H


#include "../mpc/mpc.h"
#include "../qt_eagle_xml_parser/eagle.h"

#include "kicadimportutils.h"

#include <QString>

class KicadLegacySymbolFileImport
{
public:
    KicadLegacySymbolFileImport();

    bool parseSymbolLibFile(const QString & libraryPath, Library *lib);
private:
    mpc_ast_t *parseSymbolLibFileToAst(const char *file_name, const char *string);
    KicadUnitConverter m_unitConverter;

    void parseSymbolAstToLibrary(mpc_ast_t *symbol_ast, Library *library);
    void parseDrawingAstToSymbol(mpc_ast_t *drawing_ast, Symbol *symbol);
    void rectangleAstToSymbol(mpc_ast_t *rectangle_ast, Symbol *symbol);
    void pinAstToSymbol(mpc_ast_t *pin_ast, Symbol *symbol);

    Library *m_library;
    QMap<QString, int> m_pinCountMap;
};

#endif // KICADLEGACYSYMBOLFILEIMPORT_H
