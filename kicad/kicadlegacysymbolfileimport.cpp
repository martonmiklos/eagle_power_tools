#include "kicadlegacysymbolfileimport.h"

#include "qt_eagle_xml_parser/eaglelayers.h"
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
    Devicesets *dss = new Devicesets();
    m_library->setDevicesets(dss);

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
        text->setLayer(EagleLayers::Names);
        text->setValue(">NAME");
        parseTextAstAttributes(refdes_text_ast, text);
        symbol->addText(text);

        refdesPrefix = QString(refdes_text_ast->children[1]->children[1]->contents);
    }

    mpc_ast_t *name_text = mpc_ast_get_child_lb(symbol_ast, "name_text|>", 0);
    if (name_text) {
        Text *text = new Text();
        text->setLayer(EagleLayers::Values);
        text->setValue(">VALUE");
        parseTextAstAttributes(name_text, text);

        symbol->addText(text);

        deviceName = QString(name_text->children[1]->children[1]->contents);
    }

    Connects *connects = nullptr;
    mpc_ast_t *footprint_text = mpc_ast_get_child_lb(symbol_ast, "footprint_text|>", 0);
    if (footprint_text) {
        mpc_ast_t *footprint_name_ast = mpc_ast_get_child_lb(footprint_text, "string|>", 0);
        if (footprint_name_ast) {
            QString footprintName(footprint_name_ast->children[1]->contents);
            // some footprint references are like:
            // "Housings_SSOP:TSSOP-16_4.4x5mm_Pitch0.65mm"
            if (footprintName.contains(":")) {
                footprintName = footprintName.mid(footprintName.indexOf(':') + 1);
            }

            for (Package *pkg : *m_library->packages()->packageList()) {
                if (pkg->name().toLower() == footprintName.toLower()) {
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
                    m_library->devicesets()->addDeviceset(ds);
                    break;
                }
            }
        }
    }


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

    for(int i = 0; i >= 0;) {
        i = mpc_ast_get_index_lb(drawing_ast, "polygon|>", i);
        if (i >= 0) {
            mpc_ast_t *polygon_ast = mpc_ast_get_child_lb(drawing_ast, "polygon|>", i);
            polygonAstToSymbol(polygon_ast, symbol);
            i++;
        }
    }

    for(int i = 0; i >= 0;) {
        i = mpc_ast_get_index_lb(drawing_ast, "circle|>", i);
        if (i >= 0) {
            mpc_ast_t *circle_ast = mpc_ast_get_child_lb(drawing_ast, "circle|>", i);
            circleAstToSymbol(circle_ast, symbol);
            i++;
        }
    }

    for(int i = 0; i >= 0;) {
        i = mpc_ast_get_index_lb(drawing_ast, "arc|>", i);
        if (i >= 0) {
            mpc_ast_t *arc_ast = mpc_ast_get_child_lb(drawing_ast, "arc|>", i);
            arcAstToSymbol(arc_ast, symbol);
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

    /* The fill parameter is “f” for a filled shape in the background colour, “F” for a filled shape in
    the pen colour, or “N” for an unfilled shape. */
    if (fill_ast && QString(fill_ast->contents) == "F") {
        // filled rect w pen color
        Rectangle *rect = new Rectangle();

        rect->setX1(m_unitConverter.convert(x1_ast->children[0]->contents));
        rect->setY1(m_unitConverter.convert(y1_ast->children[0]->contents));
        rect->setX2(m_unitConverter.convert(x2_ast->children[0]->contents));
        rect->setY2(m_unitConverter.convert(y2_ast->children[0]->contents));
        rect->setLayer(EagleLayers::Symbols);
        symbol->addRectangle(rect);
    } else {
        // non filled rect -> add 4 wires
        double width = m_unitConverter.convert(pen_ast->children[0]->contents);
        if (qFuzzyCompare(width, 0)) {
            /* The pen parameter is the thickness of the pen; when zero, the default pen
            width is used. */
            width = 0.254;
        }
        Wire *wire = new Wire();
        wire->setWidth(width);
        wire->setLayer(EagleLayers::Symbols);

        wire->setX1(m_unitConverter.convert(x1_ast->children[0]->contents));
        wire->setY1(m_unitConverter.convert(y1_ast->children[0]->contents));
        wire->setX2(m_unitConverter.convert(x2_ast->children[0]->contents));
        wire->setY2(m_unitConverter.convert(y1_ast->children[0]->contents));
        symbol->addWire(wire);

        wire = new Wire();
        wire->setWidth(width);
        wire->setLayer(EagleLayers::Symbols);

        wire->setX1(m_unitConverter.convert(x2_ast->children[0]->contents));
        wire->setY1(m_unitConverter.convert(y1_ast->children[0]->contents));
        wire->setX2(m_unitConverter.convert(x2_ast->children[0]->contents));
        wire->setY2(m_unitConverter.convert(y2_ast->children[0]->contents));
        symbol->addWire(wire);

        wire = new Wire();
        wire->setWidth(width);
        wire->setLayer(EagleLayers::Symbols);

        wire->setX1(m_unitConverter.convert(x2_ast->children[0]->contents));
        wire->setY1(m_unitConverter.convert(y2_ast->children[0]->contents));
        wire->setX2(m_unitConverter.convert(x1_ast->children[0]->contents));
        wire->setY2(m_unitConverter.convert(y2_ast->children[0]->contents));
        symbol->addWire(wire);

        wire = new Wire();
        wire->setWidth(width);
        wire->setLayer(EagleLayers::Symbols);

        wire->setX1(m_unitConverter.convert(x1_ast->children[0]->contents));
        wire->setY1(m_unitConverter.convert(y2_ast->children[0]->contents));
        wire->setX2(m_unitConverter.convert(x1_ast->children[0]->contents));
        wire->setY2(m_unitConverter.convert(y1_ast->children[0]->contents));
        symbol->addWire(wire);
    }
}

/*
Kicad:
INPUT	I X
OUTPUT	O X
BIDI	B X
TRISTATE	T X
PASSIVE	P X
UNSPECIFIED	U
POWER INPUT	W X
POWER OUTPUT	w X
OPEN COLLECTOR	C X
OPEN EMITTER	E
NOT CONNECTED	N X

EAGLE:
NC 		not connected X
In 		input X
Out 	output (totem-pole) X
IO 		in/output (bidirectional) X
OC 		open collector or open drain X
Hiz 	high impedance output (e.g. 3-state) X
Pas 	passive (for resistors, capacitors etc.) X
Pwr 	power input pin (Vcc, Gnd, Vss, Vdd, etc.) X
Sup 	general supply pin (e.g. for ground symbol) X
*/
QMap<QChar, Pin::DirectionEnum> KicadLegacySymbolFileImport::kicadPinDirToEaglePinDir =
{
    {'W', Pin::Direction_pwr},
    {'w', Pin::Direction_sup},
    {'I', Pin::Direction_in},
    {'O', Pin::Direction_out},
    {'B', Pin::Direction_io},
    {'C', Pin::Direction_oc},
    {'N', Pin::Direction_nc},
    {'T', Pin::Direction_hiz}
};

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
        if (kicadRot == "U") { // up
            pin->setRot("R90");
        } else if (kicadRot == "L") { // left
            pin->setRot("R180");
        } else if (kicadRot == "D") { // down
            pin->setRot("R270");
        }
        // right is 0 degrees
    }

    mpc_ast_t *pin_type_ast = mpc_ast_get_child_lb(pin_ast, "pin_type|char", 0);
    if (pin_type_ast) {
        QString pinType(pin_type_ast->contents);
        if (kicadPinDirToEaglePinDir.contains(pinType.at(0))) {
            pin->setDirection(kicadPinDirToEaglePinDir.value(pinType.at(0)));
        } else {
            // use EAGLE default here
            pin->setDirection(Pin::Direction_io);
        }

        // quickmess GND always pas FIXME remove once lib QA competed
        if (pin->name() == "GND" || pin->name() == "VSS")
            pin->setDirection(Pin::Direction_pas);

        if (pin->name() == "NC")
            pin->setDirection(Pin::Direction_nc);
    }

    mpc_ast_t *length_ast = mpc_ast_get_child_lb(pin_ast, "length|>", 0);
    if (length_ast) {
        double length = m_unitConverter.convert(length_ast->children[0]->contents);
        if (qFuzzyCompare(length, 0)) {
            pin->setLength(Pin::Length_point);
        } else if (0 < length && length <= 2.54) {
            pin->setLength(Pin::Length_short);
        } else if (2.54 < length && length <= 5.12) {
            pin->setLength(Pin::Length_middle);
        } else {
            pin->setLength(Pin::Length_long);
        }
        // TODO maybe draw the pin line further with a wire but the pin label is fixed anyway
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

                if (connects) {
                    foreach (Connect *conn, *connects->connectList()) {
                        if (conn->pin() == pin->name()) {
                            conn->setPin(existingPin->name());
                            break;
                        }
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

void KicadLegacySymbolFileImport::polygonAstToSymbol(mpc_ast_t *polygon_ast, KicadImportSymbol *symbol)
{
    int count = 0;
    mpc_ast_t *count_ast = mpc_ast_get_child(polygon_ast, "count|>");
    if (count_ast) {
        count = QString(count_ast->children[0]->contents).toInt();
        if (count < 1)
            return; // malfored polygon wo points
        Polygon *polygon = nullptr;
        double width = 0.0;

        mpc_ast_t * fill_ast = mpc_ast_get_child(polygon_ast, "fill|char");
        if (fill_ast) {
            if (QString(fill_ast->contents) == "F") {
                polygon = new Polygon();
                polygon->setLayer(EagleLayers::Symbols);
            }
        }

        mpc_ast_t * pen_ast = mpc_ast_get_child(polygon_ast, "pen|>");
        if (pen_ast) {
            width = m_unitConverter.convert(pen_ast->children[0]->contents);
            // The pen parameter is the thickness of the pen;
            // when zero, the default pen width is used.
            if (qFuzzyIsNull(width)) {
                width = 0.2032; // default is 0.008 inch
            }
            if (polygon)
                polygon->setWidth(width);
        }

        mpc_ast_t *points_ast = mpc_ast_get_child(polygon_ast, "points|>");
        qreal prevX = 0.0, prevY = 0.0;
        bool hasPrev = false;
        for (int i = 0; i<points_ast->children_num; i++) {
            mpc_ast_t *point = points_ast->children[i];
            qreal x = m_unitConverter.convert(point->children[0]->children[0]->contents);
            qreal y = m_unitConverter.convert(point->children[1]->children[0]->contents);

            if (polygon) {
                Vertex *vertex = new Vertex();
                vertex->setX(x);
                vertex->setY(y);
                vertex->setCurve(0.0);
                polygon->addVertex(vertex);
            } else {
                if (hasPrev) {
                    Wire *wire = new Wire();
                    wire->setWidth(width);

                    wire->setLayer(EagleLayers::Symbols);
                    wire->setX1(prevX);
                    wire->setY1(prevY);
                    wire->setX2(x);
                    wire->setY2(y);
                    symbol->addWire(wire);
                } else {
                    hasPrev = true;
                }
                prevX = x;
                prevY = y;
            }
        }
        if (polygon)
            symbol->addPolygon(polygon);
    }
}

void KicadLegacySymbolFileImport::circleAstToSymbol(mpc_ast_t *circle_ast, KicadImportSymbol *symbol)
{
    mpc_ast_t *x_ast = mpc_ast_get_child_lb(circle_ast, "x|>", 0);
    mpc_ast_t *y_ast = mpc_ast_get_child_lb(circle_ast, "y|>", 0);
    mpc_ast_t *r_ast = mpc_ast_get_child_lb(circle_ast, "radius|>", 0);
    mpc_ast_t *pen_ast = mpc_ast_get_child_lb(circle_ast, "pen|>", 0);
    mpc_ast_t *fill_ast = mpc_ast_get_child_lb(circle_ast, "fill|char", 0);


    if (fill_ast && QString(fill_ast->contents) == "F") {
        // filled rect w pen color -> draw polygon instead FIXME
    } else {
        Circle *circle = new Circle();
        circle->setLayer(EagleLayers::Symbols);
        circle->setX(m_unitConverter.convert(x_ast->children[0]->contents));
        circle->setY(m_unitConverter.convert(y_ast->children[0]->contents));
        circle->setRadius(m_unitConverter.convert(r_ast->children[0]->contents));

        if (pen_ast) {
            qreal width = m_unitConverter.convert(pen_ast->children[0]->contents);
            // The pen parameter is the thickness of the pen;
            // when zero, the default pen width is used.
            if (qFuzzyIsNull(width))
                width = 0.2032; // default is 0.008 inch
            circle->setWidth(width);
        }
        symbol->addCircle(circle);
    }
}

void KicadLegacySymbolFileImport::arcAstToSymbol(mpc_ast_t *arc_ast, KicadImportSymbol *symbol)
{
    mpc_ast_t *x_start_ast = mpc_ast_get_child_lb(arc_ast, "xstart|>", 0);
    mpc_ast_t *y_start_ast = mpc_ast_get_child_lb(arc_ast, "ystart|>", 0);

    mpc_ast_t *x_end_ast = mpc_ast_get_child_lb(arc_ast, "xend|>", 0);
    mpc_ast_t *y_end_ast = mpc_ast_get_child_lb(arc_ast, "yend|float|regex", 0);

    mpc_ast_t *start_angle_ast = mpc_ast_get_child_lb(arc_ast, "start_angle|>", 0);
    mpc_ast_t *end_angle_ast = mpc_ast_get_child_lb(arc_ast, "end_angle|>", 0);

    mpc_ast_t *pen_ast = mpc_ast_get_child_lb(arc_ast, "pen|>", 0);

    // "A " <x> <y> <radius> <start_angle> <end_angle> <part_index> <dmg> <pen> <fill> <xstart> <ystart> <xend> <yend> <n> ;

    Wire *wire = new Wire();
    wire->setLayer(EagleLayers::Symbols);
    if (pen_ast) {
        qreal width = m_unitConverter.convert(pen_ast->children[0]->contents);
        // The pen parameter is the thickness of the pen;
        // when zero, the default pen width is used.
        if (qFuzzyIsNull(width))
            width = 0.2032; // default is 0.008 inch
        wire->setWidth(width);
    }

    double angle = QString(end_angle_ast->children[0]->contents).toDouble() / 10.0 -
            QString(start_angle_ast->children[0]->contents).toDouble() / 10.0;
    wire->setCurve(angle);

    if (fabs(angle) > 180.0) {
        // The arc is drawn in counter-clockwise direction,
        // but the angles are swapped if there
        // (normalized) difference exceeds 180 degrees.

        wire->setX2(m_unitConverter.convert(x_start_ast->children[0]->contents));
        wire->setY2(m_unitConverter.convert(y_start_ast->children[0]->contents));
        wire->setX1(m_unitConverter.convert(x_end_ast->children[0]->contents));
        wire->setY1(m_unitConverter.convert(y_end_ast->contents));
    } else {
        wire->setX1(m_unitConverter.convert(x_start_ast->children[0]->contents));
        wire->setY1(m_unitConverter.convert(y_start_ast->children[0]->contents));
        wire->setX2(m_unitConverter.convert(x_end_ast->children[0]->contents));
        wire->setY2(m_unitConverter.convert(y_end_ast->contents));
    }

    symbol->addWire(wire);
}

Library *KicadLegacySymbolFileImport::library() const
{
    return m_library;
}

void KicadLegacySymbolFileImport::setLibrary(Library *library)
{
    m_library = library;
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
