#ifndef KICADLEGACYSYMBOLFILEIMPORT_H
#define KICADLEGACYSYMBOLFILEIMPORT_H


#include "../mpc/mpc.h"
#include "../qt_eagle_xml_parser/eagle.h"

#include "kicadimportutils.h"

#include <QString>

class KicadImportSymbol : public Symbol
{
public:
    KicadImportSymbol() : Symbol() {}
    bool pinNumbersVisible() const;
    void setPinNumbersVisible(bool pinNumbersVisible);

    bool pinNamesVisible() const;
    void setPinNamesVisible(bool pinNamesVisible);

private:
    bool m_pinNumbersVisible, m_pinNamesVisible;
};

class KicadLegacySymbolFileImport
{
public:
    KicadLegacySymbolFileImport();

    bool parseSymbolLibFile(const QString & libraryPath, Library *lib);
    Library *library() const;
    void setLibrary(Library *library);

private:
    mpc_ast_t *parseSymbolLibFileToAst(const char *file_name, const char *string);
    KicadUnitConverter m_unitConverter;

    void parseSymbolAstToLibrary(mpc_ast_t *symbol_ast);
    void parseDrawingAstToSymbol(mpc_ast_t *drawing_ast, KicadImportSymbol *symbol, Connects *connects);
    void rectangleAstToSymbol(mpc_ast_t *rectangle_ast, KicadImportSymbol *symbol);
    void pinAstToSymbol(mpc_ast_t *pin_ast, KicadImportSymbol *symbol, Connects *connects);
    void polygonAstToSymbol(mpc_ast_t *polygon_ast, KicadImportSymbol *symbol);
    void circleAstToSymbol(mpc_ast_t *circle_ast, KicadImportSymbol *symbol);
    void arcAstToSymbol(mpc_ast_t *arc_ast, KicadImportSymbol *symbol);

    Library *m_library;
    QMap<QString, int> m_pinCountMap;
    void parseTextAstAttributes(mpc_ast_t *refdes_text_ast, Text *text);

    static QMap <QChar, Pin::DirectionEnum> kicadPinDirToEaglePinDir;
};

#endif // KICADLEGACYSYMBOLFILEIMPORT_H
