#include "kicadlegacysymbolfileimport.h"


#include "legacy_symbollib_grammar.h"

#include <QDebug>

enum cleanup_length_e
{
#define X(CVAR, NAME) _ignore_me_ ## CVAR,
    X_MACRO_PARSE_VARS
    CLEANUP_LENGTH
#undef X
};


KicadLegacySymbolFileImport::KicadLegacySymbolFileImport()
{

}

bool KicadLegacySymbolFileImport::parseSymbolLibFile(const QString &libraryPath, Library *lib)
{
    m_unitConverter.setMode(KicadUnitConverter::Mil);
    m_library = lib;
    mpc_ast_t *ast = nullptr;
    FILE *handle = fopen(libraryPath.toLocal8Bit().constData(), "rb");
    char *istring = nullptr;

    if(!handle)
        return false;


    if(!(istring = slurp(handle))) {
        free(istring);
        return false;
    }

    ast = parseSymbolLibFileToAst(libraryPath.toLocal8Bit().constData(), istring);

    for(int i = 0; i >= 0;) {
        i = mpc_ast_get_index_lb(ast, "symbol|>", i);
        if (i >= 0) {
            mpc_ast_t *symbol_ast = mpc_ast_get_child_lb(ast, "symbol|>", i);
            parseSymbolAstToLibrary(symbol_ast, m_library);
            i++;
        }
    }
    return true;
}


mpc_ast_t *KicadLegacySymbolFileImport::parseSymbolLibFileToAst(const char *file_name, const char *data)
{
    Q_ASSERT(file_name);
    Q_ASSERT(data);
#define X(CVAR, NAME) mpc_parser_t *CVAR = mpc_new((NAME));
    X_MACRO_PARSE_VARS
        #undef X

        #define X(CVAR, NAME) CVAR,
            mpc_err_t *language_error = mpca_lang_contents(
                MPCA_LANG_WHITESPACE_SENSITIVE,
                "/home/mm/Projektek/qt/eagle_power_tools/kicad/symbollib_grammar.txt", // FIXME
                X_MACRO_PARSE_VARS NULL);
#undef X

    if (language_error != nullptr) {
        mpc_err_print(language_error);
        mpc_err_delete(language_error);
        return nullptr;
    }

    mpc_result_t r;
    mpc_ast_t *ast = nullptr;
    if (mpc_parse(file_name, data, symbol_lib, &r)) {
        ast = static_cast<mpc_ast_t*>(r.output);
    } else {
        mpc_err_print(r.error);
        mpc_err_delete(r.error);
    }

#define X(CVAR, NAME) CVAR,
    mpc_cleanup(CLEANUP_LENGTH,
                X_MACRO_PARSE_VARS NULL
                );
#undef X
    return ast;
}

void KicadLegacySymbolFileImport::parseSymbolAstToLibrary(mpc_ast_t *symbol_ast, Library *library)
{
    Symbol *symbol = new Symbol();
    QString footprintName;

    mpc_ast_t *symboldef_ast = mpc_ast_get_child_lb(symbol_ast, "symboldef|>", 0);
    if (symboldef_ast) {
        mpc_ast_t *name_ast = mpc_ast_get_child_lb(symboldef_ast, "symbolname|>", 0);
        if (name_ast) {
            symbol->setName(name_ast->children[0]->contents);
        }
    }

    mpc_ast_t *drawing_ast = mpc_ast_get_child_lb(symbol_ast, "drawing|>", 0);
    if (drawing_ast) {
        parseDrawingAstToSymbol(drawing_ast, symbol);
    }


    // TODO generate deviceset
    mpc_ast_t *footprint_text_ast = mpc_ast_get_child_lb(symbol_ast, "footprint_text|>", 0);
    if (footprint_text_ast && footprint_text_ast->children_num > 2) {
        footprintName = footprint_text_ast->children[1]->contents;
    }

    m_library->symbols()->addSymbol(symbol);
}

void KicadLegacySymbolFileImport::parseDrawingAstToSymbol(mpc_ast_t *drawing_ast, Symbol *symbol)
{
    m_pinCountMap.clear();
    for(int i = 0; i >= 0;) {
        i = mpc_ast_get_index_lb(drawing_ast, "rectangle|>", i);
        if (i >= 0) {
            mpc_ast_t *rectangle_ast = mpc_ast_get_child_lb(drawing_ast, "rectangle|>", i);
            rectangleAstToSymbol(rectangle_ast, symbol);
            i++;
        }
    }

    for(int i = 0; i >= 0;) {
        i = mpc_ast_get_index_lb(drawing_ast, "pin|>", i);
        if (i >= 0) {
            mpc_ast_t *pin_ast = mpc_ast_get_child_lb(drawing_ast, "pin|>", i);
            pinAstToSymbol(pin_ast, symbol);
            i++;
        }
    }
}

void KicadLegacySymbolFileImport::rectangleAstToSymbol(mpc_ast_t *rectangle_ast, Symbol *symbol)
{

    mpc_ast_t *x1_ast = mpc_ast_get_child_lb(rectangle_ast, "x1|>", 0);
    mpc_ast_t *y1_ast = mpc_ast_get_child_lb(rectangle_ast, "y1|>", 0);
    mpc_ast_t *x2_ast = mpc_ast_get_child_lb(rectangle_ast, "x2|>", 0);
    mpc_ast_t *y2_ast = mpc_ast_get_child_lb(rectangle_ast, "y2|>", 0);
    mpc_ast_t *pen_ast = mpc_ast_get_child_lb(rectangle_ast, "pen|>", 0);
    mpc_ast_t *fill_ast = mpc_ast_get_child_lb(rectangle_ast, "fill|char", 0);

    if (QString(fill_ast->contents) == "N") {
        // non filled rect -> add 4 wires
        double width = m_unitConverter.convert(pen_ast->children[0]->contents);
        if (qFuzzyCompare(width, 0)) {
            /* The pen parameter is the thickness of the pen; when zero, the default pen
            width is used. */
            width = 0.254;
        }
        Wire *wire = new Wire();
        wire->setWidth(width);
        wire->setLayer(94);

        wire->setX1(m_unitConverter.convert(x1_ast->children[0]->contents));
        wire->setY1(m_unitConverter.convert(y1_ast->children[0]->contents));
        wire->setX2(m_unitConverter.convert(x2_ast->children[0]->contents));
        wire->setY2(m_unitConverter.convert(y1_ast->children[0]->contents));
        symbol->addWire(wire);

        wire = new Wire();
        wire->setWidth(width);
        wire->setLayer(94);

        wire->setX1(m_unitConverter.convert(x2_ast->children[0]->contents));
        wire->setY1(m_unitConverter.convert(y1_ast->children[0]->contents));
        wire->setX2(m_unitConverter.convert(x2_ast->children[0]->contents));
        wire->setY2(m_unitConverter.convert(y2_ast->children[0]->contents));
        symbol->addWire(wire);

        wire = new Wire();
        wire->setWidth(width);
        wire->setLayer(94);

        wire->setX1(m_unitConverter.convert(x2_ast->children[0]->contents));
        wire->setY1(m_unitConverter.convert(y2_ast->children[0]->contents));
        wire->setX2(m_unitConverter.convert(x1_ast->children[0]->contents));
        wire->setY2(m_unitConverter.convert(y2_ast->children[0]->contents));
        symbol->addWire(wire);

        wire = new Wire();
        wire->setWidth(width);
        wire->setLayer(94);

        wire->setX1(m_unitConverter.convert(x1_ast->children[0]->contents));
        wire->setY1(m_unitConverter.convert(y2_ast->children[0]->contents));
        wire->setX2(m_unitConverter.convert(x1_ast->children[0]->contents));
        wire->setY2(m_unitConverter.convert(y1_ast->children[0]->contents));
        symbol->addWire(wire);
    } else {
        // filled rect
        Rectangle *rect = new Rectangle();

        rect->setX1(m_unitConverter.convert(x1_ast->children[0]->contents));
        rect->setY1(m_unitConverter.convert(y1_ast->children[0]->contents));
        rect->setX2(m_unitConverter.convert(x2_ast->children[0]->contents));
        rect->setY2(m_unitConverter.convert(y2_ast->children[0]->contents));
        rect->setLayer(94); // FIXME
        symbol->addRectangle(rect);
    }
}

void KicadLegacySymbolFileImport::pinAstToSymbol(mpc_ast_t *pin_ast, Symbol *symbol)
{
    Pin *pin = new Pin();
    mpc_ast_t *name_ast = mpc_ast_get_child_lb(pin_ast, "nonquoted_name_string|regex", 0);
    if (name_ast)
        pin->setName(name_ast->contents);

    mpc_ast_t *x_ast = mpc_ast_get_child_lb(pin_ast, "x|>", 0);
    mpc_ast_t *y_ast = mpc_ast_get_child_lb(pin_ast, "y|>", 0);

    pin->setX(m_unitConverter.convert(x_ast->children[0]->contents));
    pin->setY(m_unitConverter.convert(y_ast->children[0]->contents));

    mpc_ast_t *orientation = mpc_ast_get_child_lb(pin_ast, "pin_orientation|>", 0);
    if (orientation) {
        QString kicadRot(orientation->children[0]->contents);
        if (kicadRot == "D") { // down
            pin->setRot("R90");
        } else if (kicadRot == "L") { // left
            pin->setRot("R180");
        } else if (kicadRot == "T") { // top
            pin->setRot("R270");
        }
    }

    if (m_pinCountMap.contains(pin->name())) {
        QString baseName = pin->name();
        pin->setName(QString("%1@%2").arg(baseName).arg(m_pinCountMap[baseName]));
        m_pinCountMap[baseName] = m_pinCountMap[baseName] + 1;
    } else {
        for (Pin *existingPin : *symbol->pinList()) {
            if (existingPin->name() == pin->name()) {
                existingPin->setName(existingPin->name() + "@1");
                m_pinCountMap[pin->name()] = 3;
                pin->setName(pin->name() + "@2");
                break;
            }
        }
    }

    symbol->addPin(pin);
}
