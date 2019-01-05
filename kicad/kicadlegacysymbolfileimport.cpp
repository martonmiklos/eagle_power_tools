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

    Symbols *symbols = new Symbols();
    m_library->setSymbols(symbols);
    ast = parseSymbolLibFileToAst(libraryPath.toLocal8Bit().constData(), istring);
    for(int i = 0; i >= 0;) {
        i = mpc_ast_get_index_lb(ast, "symbol|>", i);
        if (i >= 0) {
            mpc_ast_t *symbol_ast = mpc_ast_get_child_lb(ast, "symbol|>", i);
            parseSymbolAstToLibrary(symbol_ast);
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

void KicadLegacySymbolFileImport::parseTextAstAttributes(mpc_ast_t *text_ast, Text *text)
{
    mpc_ast_t *x_ast = mpc_ast_get_child_lb(text_ast, "x|>", 0);
    mpc_ast_t *y_ast = mpc_ast_get_child_lb(text_ast, "y|>", 0);

    text->setX(m_unitConverter.convert(x_ast->children[0]->contents));
    text->setY(m_unitConverter.convert(y_ast->children[0]->contents));

    mpc_ast_t *size_ast = mpc_ast_get_child_lb(text_ast, "size|>", 0);
    text->setSize(m_unitConverter.convert(size_ast->children[0]->contents));

    mpc_ast_t *orientation = mpc_ast_get_child_lb(text_ast, "orientation|>", 0);
    if (QString(orientation->children[0]->contents) == "V")
        text->setRot("R270");

    mpc_ast_t *halign = mpc_ast_get_child_lb(text_ast, "halign|>", 0);
    mpc_ast_t *valign = mpc_ast_get_child_lb(text_ast, "valign|char", 0);

    QString halignStr = QString(halign->children[0]->contents);
    QString valignStr = QString(valign->contents);
    if (valignStr == "T") {
        if (halignStr == "L") {
            text->setAlign(Text::Align_top_left);
        } else if (halignStr == "C") {
            text->setAlign(Text::Align_top_center);
        } else if (halignStr == "R") {
            text->setAlign(Text::Align_top_right);
        }
    } else if (valignStr == "C") {
        if (halignStr == "L") {
            text->setAlign(Text::Align_center_left);
        } else if (halignStr == "C") {
            text->setAlign(Text::Align_center);
        } else if (halignStr == "R") {
            text->setAlign(Text::Align_center_right);
        }
    } else if (valignStr == "B") {
        if (halignStr == "L") {
            text->setAlign(Text::Align_bottom_left);
        } else if (halignStr == "C") {
            text->setAlign(Text::Align_bottom_center);
        } else if (halignStr == "R") {
            text->setAlign(Text::Align_bottom_right);
        }
    }
}

void KicadLegacySymbolFileImport::parseSymbolAstToLibrary(mpc_ast_t *symbol_ast)
{
    KicadImportSymbol *symbol = new KicadImportSymbol();
    QString footprintName, refdesPrefix, deviceName;

    mpc_ast_t *symboldef_ast = mpc_ast_get_child_lb(symbol_ast, "symboldef|>", 0);
    if (symboldef_ast) {
        mpc_ast_t *name_ast = mpc_ast_get_child_lb(symboldef_ast, "symbolname|>", 0);
        if (name_ast) {
            symbol->setName(name_ast->children[0]->contents);
        }

        mpc_ast_t *show_pin_names = mpc_ast_get_child_lb(symboldef_ast, "show_pin_names|>", 0);
        if (show_pin_names) {
            symbol->setPinNamesVisible(QString(show_pin_names->children[0]->contents) == "Y");
        }

        mpc_ast_t *show_pin_numbers = mpc_ast_get_child_lb(symboldef_ast, "show_pin_numbers|>", 0);
        if (show_pin_numbers) {
            symbol->setPinNumbersVisible(QString(show_pin_numbers->children[0]->contents) == "Y");
        }
    }

    mpc_ast_t *refdes_text_ast = mpc_ast_get_child_lb(symbol_ast, "refdes_text|>", 0);
    if (refdes_text_ast) {
        Text *text = new Text();
        text->setLayer(95); // FIXME hardcoded NAMES layer
        text->setValue(">NAME");
        parseTextAstAttributes(refdes_text_ast, text);
        symbol->addText(text);

        refdesPrefix = QString(refdes_text_ast->children[1]->children[1]->contents);
    }

    mpc_ast_t *name_text = mpc_ast_get_child_lb(symbol_ast, "name_text|>", 0);
    if (name_text) {
        Text *text = new Text();
        text->setLayer(96); // FIXME hardcoded VALUES layer
        text->setValue(">VALUE");
        parseTextAstAttributes(name_text, text);

        symbol->addText(text);

        deviceName = QString(name_text->children[1]->children[1]->contents);
    }

    Devicesets *dss = new Devicesets();
    Connects *connects = nullptr;
    mpc_ast_t *footprint_text = mpc_ast_get_child_lb(symbol_ast, "footprint_text|>", 0);
    if (footprint_text) {
        mpc_ast_t *footprint_name_ast = mpc_ast_get_child_lb(footprint_text, "string|>", 0);
        if (footprint_name_ast) {
            QString footprintName(footprint_name_ast->children[1]->contents);
            for (Package *pkg : *m_library->packages()->packageList()) {
                if (pkg->name() == footprintName) {
                    Deviceset *ds = new Deviceset();
                    ds->setPrefix(refdesPrefix);
                    ds->setName(deviceName);

                    Gate *gate = new Gate();
                    gate->setSymbol(symbol->name());
                    gate->setName("G$1");

                    Gates *gates = new Gates();
                    gates->addGate(gate);
                    ds->setGates(gates);

                    Device *dev = new Device();
                    dev->setPackage(footprintName);

                    connects = new Connects();
                    dev->setConnects(connects);

                    Devices *devs = new Devices();
                    ds->setDevices(devs);
                    ds->devices()->addDevice(dev);
                    dss->addDeviceset(ds);
                    break;
                }
            }
        }
    }
    m_library->setDevicesets(dss);

    mpc_ast_t *drawing_ast = mpc_ast_get_child_lb(symbol_ast, "drawing|>", 0);
    if (drawing_ast) {
        parseDrawingAstToSymbol(drawing_ast, symbol, connects);
    }


    m_library->symbols()->addSymbol(symbol);
}

void KicadLegacySymbolFileImport::parseDrawingAstToSymbol(mpc_ast_t *drawing_ast,
                                                          KicadImportSymbol *symbol,
                                                          Connects *connects)
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
            pinAstToSymbol(pin_ast, symbol, connects);
            i++;
        }
    }
}

void KicadLegacySymbolFileImport::rectangleAstToSymbol(mpc_ast_t *rectangle_ast, KicadImportSymbol *symbol)
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

void KicadLegacySymbolFileImport::pinAstToSymbol(mpc_ast_t *pin_ast,
                                                 KicadImportSymbol *symbol,
                                                 Connects *connects)
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

    mpc_ast_t *pin_type_ast = mpc_ast_get_child_lb(pin_ast, "pin_type|char", 0);
    if (pin_type_ast) {
        QString pinType(pin_type_ast->contents);
        if (pinType == "w") {
            // power output
            pin->setDirection(Pin::Direction_pwr);
        } else if (pinType == "w") {
            // power input
            pin->setDirection(Pin::Direction_sup);
        } else if (pinType == "I") {
            pin->setDirection(Pin::Direction_in);
        } else if (pinType == "O") {
            pin->setDirection(Pin::Direction_out);
        } else if (pinType == "B") {
            pin->setDirection(Pin::Direction_io);
        } else if (pinType == "C") {
            pin->setDirection(Pin::Direction_oc);
        } else if (pinType == "N") {
            pin->setDirection(Pin::Direction_nc);
        } else {
            pin->setDirection(Pin::Direction_pas);
        }

        // quickmess GND always pas
        if (pin->name() == "GND")
            pin->setDirection(Pin::Direction_pas);
    }

    if (symbol->pinNamesVisible() && symbol->pinNumbersVisible())
        pin->setVisible(Pin::Visible_both);
    else if (!symbol->pinNamesVisible() && symbol->pinNumbersVisible())
        pin->setVisible(Pin::Visible_pad);
    else if (symbol->pinNamesVisible() && !symbol->pinNumbersVisible())
        pin->setVisible(Pin::Visible_pin);
    else if (!symbol->pinNamesVisible() && !symbol->pinNumbersVisible())
        pin->setVisible(Pin::Visible_off);

    if (m_pinCountMap.contains(pin->name())) {
        QString baseName = pin->name();
        pin->setName(QString("%1@%2").arg(baseName).arg(m_pinCountMap[baseName]));
        m_pinCountMap[baseName] = m_pinCountMap[baseName] + 1;
    } else {
        for (Pin *existingPin : *symbol->pinList()) {
            if (existingPin->name() == pin->name()) {
                existingPin->setName(existingPin->name() + "@1");

                foreach (Connect *conn, *connects->connectList()) {
                    if (conn->pin() == pin->name()) {
                        conn->setPin(existingPin->name());
                        break;
                    }
                }

                m_pinCountMap[pin->name()] = 3;
                pin->setName(pin->name() + "@2");
                break;
            }
        }
    }

    if (connects) {
        Connect *conn = new Connect();
        conn->setGate("G$1"); // FIXME
        mpc_ast_t *pin_number = mpc_ast_get_child_lb(pin_ast, "pin_number|>", 0);
        if (pin_number) {
            QString pinNumberStr(pin_number->children[0]->contents);
            if (pinNumberStr != "~") {
                conn->setPad(pinNumberStr);
                conn->setPin(pin->name());
                connects->addConnect(conn);
            }
        }
    }

    symbol->addPin(pin);
}

bool KicadImportSymbol::pinNumbersVisible() const
{
    return m_pinNumbersVisible;
}

void KicadImportSymbol::setPinNumbersVisible(bool pinNumbersVisible)
{
    m_pinNumbersVisible = pinNumbersVisible;
}

bool KicadImportSymbol::pinNamesVisible() const
{
    return m_pinNamesVisible;
}

void KicadImportSymbol::setPinNamesVisible(bool pinNamesVisible)
{
    m_pinNamesVisible = pinNamesVisible;
}
