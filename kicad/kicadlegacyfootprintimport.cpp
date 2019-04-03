#include "kicadlegacyfootprintimport.h"

#include "unitutilities.h"
#include "qt_eagle_xml_parser/eaglelayers.h"

#include <string.h>
#include <stdio.h>

#include <QDebug>
#include <QFile>
#include <QFileInfo>


#define X_MACRO_PARSE_VARS\
    X(spaces,               "s")\
    X(newline,              "n")\
    X(sign,                 "sign")\
    X(flt,                  "float")\
    X(integer,              "integer")\
    X(strings,              "string")\
    X(nonquoted_string,              "nonquoted_string")\
    X(hexint, "hexint")\
    X(floatp, "float")\
    X(header, "header")\
    X(encoding, "encoding")\
    X(symbolname, "symbolname")\
    X(symbolnames, "symbolnames")\
    X(module, "module")\
    X(x, "x")\
    X(y, "y")\
    X(xdelta, "xdelta")\
    X(ydelta, "ydelta")\
    X(width, "width")\
    X(height, "height")\
    X(x1, "x1")\
    X(y1, "y1")\
    X(x2, "x2")\
    X(y2, "y2")\
    X(angle, "angle")\
    X(layer, "layer")\
    X(tstamp, "tstamp")\
    X(tedit, "tedit")\
    X(modattrs, "modattrs")\
    X(moddescription, "moddescription")\
    X(modtype, "modtype")\
    X(padtype, "padtype")\
    X(alternate_ref, "alternate_ref")\
    X(pen90, "pen90")\
    X(pen180, "pen180")\
    X(orientation_penality, "orientation_penality")\
    X(pen, "pen")\
    X(mirror, "mirror")\
    X(visible, "visible")\
    X(italic, "italic")\
    X(reference_text, "reference_text")\
    X(value_text, "value_text")\
    X(user_text, "user_text")\
    X(line, "line")\
    X(lines, "lines")\
    X(circle, "circle")\
    X(circles, "circles")\
    X(arc, "arc")\
    X(arcs, "arcs")\
    X(polygonpt, "polygonpt")\
    X(polygon, "polygon")\
    X(polygons, "polygons")\
    X(shape, "shape")\
    X(padpos, "padpos")\
    X(padname, "padname")\
    X(pad_die_length, "pad_die_length")\
    X(net_number, "net_number")\
    X(pad, "pad")\
    X(pads, "pads")\
    X(keywords, "keywords")\
    X(modulepos, "modulepos")\
    X(endlibrary, "endlibrary")\
    X(units, "units")\
    X(library, "library")\
    X(orientation, "orientation")\
    X(sizep, "size")\
    X(hole, "hole")\
    X(mask, "mask")\
    X(modules, "modules")\
    X(name, "name")

enum cleanup_length_e
{
#define X(CVAR, NAME) _ignore_me_ ## CVAR,
    X_MACRO_PARSE_VARS
    CLEANUP_LENGTH
#undef X
};

// TODO
// move to external file
// cleanup: merge \"Op\" <s> to \"Op \"
static const char *modfile_grammar =
        " s                    : /[ \\t]/ ; \n"
        " n                    : /\\r?\\n/ ; \n"
        " sign                 : '+' | '-' ; \n"
        " string               : '\"' /[^\"]*/ '\"' ; \n "
        " nonquoted_string     : /[^\\r\\n$]*/ ; \n "
        " integer              : <sign>? /[0-9]+/ ; \n"
        " hexint               : /[0-9A-Fa-f]+/ ; \n"
        " float                : /[-+]?[0-9]+(\\.[0-9]+)?([eE][-+]?[0-9]+)?/ ; \n"
        " header               : \"PCBNEW-LibModule-V1\" <s> /.*/ <n> ; \n"
        " encoding             : '#' <s>* \"encoding\" <s>* /.*/ <n> ; \n"
        " symbolname           : <nonquoted_string> <n> ; \n"
        " symbolnames          : \"$INDEX\" <n> <symbolname>* \"$EndINDEX\" <n>; \n"
        " module               : \"$MODULE\" <s> <symbolname> "
        "                         <modulepos> "
        "                         \"Li\" <s> <symbolname>"
        "                         <moddescription>? "
        "                         <keywords>? "
        "                         \"Sc\" <s> <tstamp> <n>"
        "                         <modtype>? "
        "                         <alternate_ref>? "
        "                         <orientation_penality> "
        "                         <reference_text> "
        "                         <value_text> "
        "                         <user_text>? "
        "                         <lines> "
        "                         <circles> "
        "                         <polygons> "
        "                         <pads> "
        "                         \"$EndMODULE\" <s> <symbolname>; \n"
        " x                    : <float> <s>* ; \n"
        " y                    : <float> <s>* ; \n"
        " xdelta               : <float> <s>* ; \n"
        " ydelta               : <float> <s>* ; \n"
        " width                : <float> <s>* ; \n"
        " height               : <float> <s>* ; \n"
        " size                 : <float> <s>* ; \n"
        " x1                   : <float> <s>* ; \n"
        " y1                   : <float> <s>* ; \n"
        " x2                   : <float> <s>* ; \n"
        " y2                   : <float> <s>* ; \n"
        " angle                : <float> <s>* ; \n"
        " layer                : <integer> <s>* ; \n"
        " tstamp               : <hexint> <s>* ; \n"
        " tedit                : <hexint> <s>* ; \n"
        " modattrs             : (\"P\"|\"~\") \"~\" ; \n"
        " moddescription       : \"Cd\" <s> <nonquoted_string> <n>; \n"
        " modtype              : \"At\" <s> (\"STD\"|\"SMD\") <n>; \n"
        " mask                 : <hexint>; \n"
        " padtype              : \"At\" <s> (\"STD\"|\"SMD\"|\"CONN\"|\"HOLE\") <s> 'N' <s> <mask> <n>; \n"
        " alternate_ref        : \"AR\" <s> <nonquoted_string> <n>; \n"
        " pen90                : <integer>; \n"
        " pen180               : <integer>; \n"
        " orientation_penality : \"Op\" <s> <pen90> <s> <pen180> <s> \"0\" <n>; \n"
        " pen                  : <integer> <s>*; \n"
        " mirror               : (\"M\"|\"N\") <s>*; \n"
        " visible              : (\"V\"|\"I\") <s>*; \n"
        " italic               : (\"N\"|\"I\") <s>*; \n"
        " reference_text       : \"T0\" <s> <x> <y> <height> <width> <angle> <pen> <mirror> "
        "                         <visible> <layer> <italic> <string> <n>;  \n"
        " value_text           : \"T1\" <s> <x> <y> <height> <width> <angle> <pen> <mirror> "
        "                         <visible> <layer> <italic> <string> <n>;  \n"
        " user_text            : \"T2\" <s> <x> <y> <height> <width> <angle> <pen> <mirror> "
        "                         <visible> <layer> <italic> <string> <n>;  \n"
        " line                 : \"DS\" <s> <x1> <y1> <x2> <y2> <pen> <layer> <n>; \n"
        " lines                : <line>*; \n"
        " circle               : \"DC\" <s> <x> <y> <x1> <y1> <pen> <layer> <n>; \n"
        " circles              : <circle>*; \n"
        " arc                  : \"DA\" <s> <x> <y> <x1> <y1> <angle> <pen> <layer> <n>; \n"
        " arcs                 : <arc>*; \n"
        " hole                 : \"Dr\" <s> <size> <x> <y> <n>; \n"
        " polygonpt            : \"Dl\" <s> <x> <y> <n>; \n"
        " polygon              : \"DP\" <s> <x> <y> <x1> <y1> <angle> <pen> <layer> <n> <polygonpt>* <n>; \n"
        " polygons             : <polygon>*; \n"
        " name                 : <string> <s>* ; \n"
        " orientation          : <float> <s>* ; \n"
        " shape                : (('C' | 'R' | 'O' | 'T')) <s>*; \n"
        " padpos               : \"Po\" <s> <x> <y> <n>; \n"
        " padname              : \"Sh\" <s> <name> <shape> <width> <height> <ydelta> <xdelta> <orientation> <n>; \n"
        " pad_die_length       : \"Le\" <s> <float> <n>; \n"
        " net_number           : \"Ne\" <s> <integer> <s> <string> <n>; \n"
        " pad                  : \"$PAD\" <n> "
        "                         (<padpos>|"
        "                         <padname>|"
        "                         <hole>|"
        "                         <padtype>|"
        "                         <pad_die_length>|"
        "                         <net_number>)*"
        "                      \"$EndPAD\"<n>; \n"
        " pads                 : <pad>*; \n"
        " keywords             : \"Kw\" <s> (<string> <s>)* <n>; \n"
        " modulepos            : \"Po\" <s> <x> <y> <angle> <layer> <tedit> <tstamp> <modattrs> <n>; \n"
        " endlibrary           : \"$EndLIBRARY\" <n>* ; \n"
        " units                : \"Units\" <s> \"mm\" <n> ; \n"
        " library              : <header> <encoding> <units>? <symbolnames> <module>* <endlibrary>; \n" ;

KicadLegacyFootprintLibImporter::KicadLegacyFootprintLibImporter()
{
    m_eagle = new Eagle();
    m_drawing = new Drawing();
    m_eagle->setDrawing(m_drawing);
}

mpc_ast_t *KicadLegacyFootprintLibImporter::parseModfileToAst(const char *file_name, const char *string)
{
    Q_ASSERT(file_name);
    Q_ASSERT(string);
#define X(CVAR, NAME) mpc_parser_t *CVAR = mpc_new((NAME));
    X_MACRO_PARSE_VARS
        #undef X

        #define X(CVAR, NAME) CVAR,
            mpc_err_t *language_error = mpca_lang(
                MPCA_LANG_WHITESPACE_SENSITIVE,
                modfile_grammar,
                X_MACRO_PARSE_VARS NULL);
#undef X

    if (language_error != nullptr) {
        mpc_err_print(language_error);
        mpc_err_delete(language_error);
        return nullptr;
    }

    mpc_result_t r;
    mpc_ast_t *ast = nullptr;
    if (mpc_parse(file_name, string, library, &r)) {
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


Library* KicadLegacyFootprintLibImporter::parseModFile(const QString &libraryPath)
{
    m_libPath = libraryPath;

    mpc_ast_t *ast = nullptr;
    FILE *handle = fopen(libraryPath.toLocal8Bit().constData(), "rb");
    char *istring = nullptr;

    Library *lib = new Library();
    if(!handle)
        return nullptr;


    if(!(istring = slurp(handle))) {
        free(istring);
        return nullptr;
    }

    ast = parseModfileToAst(libraryPath.toLocal8Bit().constData(), istring);

    mpc_ast_t *units_ast = mpc_ast_get_child_lb(ast, "units|>", 0);
    if (units_ast) {
        m_unitConverter.setMode(KicadUnitConverter::Mm);
    }

    Packages *packages = new Packages();
    for(int i = 0; i >= 0;) {
        i = mpc_ast_get_index_lb(ast, "module|>", i);
        if (i >= 0) {
            mpc_ast_t *module_ast = mpc_ast_get_child_lb(ast, "module|>", i);
            if (module_ast) {
                mpc_ast_t *modulename_ast = mpc_ast_get_child_lb(module_ast, "symbolname|>", 0);
                if (modulename_ast) {
                    mpc_ast_t *symbolname_ast = mpc_ast_get_child_lb(modulename_ast, "nonquoted_string|regex", 0);
                    if (symbolname_ast) {
                        qWarning() << symbolname_ast->contents;

                        Package *package = parseModuleAstToPackage(module_ast, symbolname_ast->contents);
                        if (package) {
                            packages->addPackage(package);
                        }
                    }
                }
            }
            i++;
        }
    }
    lib->setPackages(packages);
    m_drawing->setLibrary(lib);

    free(istring);
    return lib;
}

void KicadLegacyFootprintLibImporter::save()
{
    QFileInfo libInfo(m_libPath);
    m_eagle->writeFile(QString("/tmp/%1.lbr").arg(libInfo.baseName()));
}

Package *KicadLegacyFootprintLibImporter::parseModuleAstToPackage(mpc_ast_t *module_ast, const char *name)
{
    Package *package = new Package();
    package->setName(name);

    mpc_ast_t *desc_ast = mpc_ast_get_child_lb(module_ast, "moddescription|>", 0);
    if (desc_ast) {
        Description *desc = new Description();
        desc->setValue(desc_ast->contents);
        package->setDescription(desc);
    }
    // TODO append keywords to description

    mpc_ast_t *refdestext_ast = mpc_ast_get_child_lb(module_ast, "reference_text|>", 0);
    if (refdestext_ast) {
        Text *refdestext = new Text();
        mpc_ast_t *x_ast = mpc_ast_get_child_lb(refdestext_ast, "x|>", 0);
        mpc_ast_t *y_ast = mpc_ast_get_child_lb(refdestext_ast, "y|>", 0);
        refdestext->setX(m_unitConverter.convertTo_mm(x_ast->children[0]->contents));
        refdestext->setY(-m_unitConverter.convertTo_mm(y_ast->children[0]->contents));

        mpc_ast_t *angle_ast = mpc_ast_get_child_lb(refdestext_ast, "angle|>", 0);
        if (angle_ast) {
            double angle = QString(angle_ast->children[0]->contents).toDouble();
            if (!qFuzzyCompare(angle, 0)) {
                refdestext->setRot(QString("R%1").arg(angle));
            }
        }

        mpc_ast_t *mirror_ast = mpc_ast_get_child_lb(refdestext_ast, "mirror|>", 0);
        int layer = 25; // FIXME
        if (mirror_ast && mirror_ast->children[0]->contents[0] == 'M') {
            layer = 26; // FIXME
        }

        mpc_ast_t *height_ast = mpc_ast_get_child_lb(refdestext_ast, "height|>", 0);
        if (height_ast) {
            refdestext->setSize(m_unitConverter.convertTo_mm(height_ast->children[0]->contents));
        }

        refdestext->setAlign(Text::Align_center);
        refdestext->setLayer(layer);
        refdestext->setValue(">NAME");
        package->addText(refdestext);
    }

    mpc_ast_t *valuetext_ast = mpc_ast_get_child_lb(module_ast, "value_text|>", 0);
    if (valuetext_ast) {
        Text *valuetext = new Text();
        mpc_ast_t *x_ast = mpc_ast_get_child_lb(valuetext_ast, "x|>", 0);
        mpc_ast_t *y_ast = mpc_ast_get_child_lb(valuetext_ast, "y|>", 0);
        valuetext->setX(m_unitConverter.convertTo_mm(x_ast->children[0]->contents));
        valuetext->setY(-m_unitConverter.convertTo_mm(y_ast->children[0]->contents));

        mpc_ast_t *angle_ast = mpc_ast_get_child_lb(valuetext_ast, "angle|>", 0);
        if (angle_ast) {
            double angle = QString(angle_ast->children[0]->contents).toDouble();
            if (!qFuzzyCompare(angle, 0)) {
                valuetext->setRot(QString("R%1").arg(angle));
            }
        }

        mpc_ast_t *mirror_ast = mpc_ast_get_child_lb(valuetext_ast, "mirror|>", 0);
        int layer = 27; // FIXME
        if (mirror_ast && mirror_ast->children[0]->contents[0] == 'M') {
            layer = 28; // FIXME
        }
        valuetext->setAlign(Text::Align_center);
        valuetext->setLayer(layer);
        valuetext->setValue(">VALUE");

        mpc_ast_t *height_ast = mpc_ast_get_child_lb(valuetext_ast, "height|>", 0);
        if (height_ast) {
            valuetext->setSize(m_unitConverter.convertTo_mm(height_ast->children[0]->contents));
        }
        package->addText(valuetext);
    }
    // TODO add user text as a visible attribute

    parseCirclesAstToPackage(mpc_ast_get_child_lb(module_ast, "circles|>", 0), package);
    parseLinesAstToPackage(mpc_ast_get_child_lb(module_ast, "lines|>", 0), package);
    parseLinesAstToPackage(mpc_ast_get_child_lb(module_ast, "polygons|>", 0), package);
    parsePadsAstToPackage(mpc_ast_get_child_lb(module_ast, "pads|>", 0), package);

    return package;
}

void KicadLegacyFootprintLibImporter::parseCirclesAstToPackage(mpc_ast_t *circles_ast, Package *package)
{
    if (circles_ast) {

    }
}

void KicadLegacyFootprintLibImporter::parseLinesAstToPackage(mpc_ast_t *lines_ast, Package *package)
{
    if (lines_ast) {
        for(int i = 0; i >= 0;) {
            i = mpc_ast_get_index_lb(lines_ast, "line|>", i);
            if (i >= 0) {
                mpc_ast_t *line_ast = mpc_ast_get_child_lb(lines_ast, "line|>", i);
                if (line_ast) {
                    Wire *line = new Wire();
                    mpc_ast_t *x1_ast = mpc_ast_get_child_lb(line_ast, "x1|>", 0);
                    mpc_ast_t *y1_ast = mpc_ast_get_child_lb(line_ast, "y1|>", 0);
                    line->setX1(m_unitConverter.convertTo_mm(x1_ast->children[0]->contents));
                    line->setY1(-m_unitConverter.convertTo_mm(y1_ast->children[0]->contents));

                    mpc_ast_t *x2_ast = mpc_ast_get_child_lb(line_ast, "x2|>", 0);
                    mpc_ast_t *y2_ast = mpc_ast_get_child_lb(line_ast, "y2|>", 0);
                    line->setX2(m_unitConverter.convertTo_mm(x2_ast->children[0]->contents));
                    line->setY2(-m_unitConverter.convertTo_mm(y2_ast->children[0]->contents));

                    mpc_ast_t *pen_ast = mpc_ast_get_child_lb(line_ast, "pen|>", 0);
                    line->setWidth(m_unitConverter.convertTo_mm(pen_ast->children[0]->contents));

                    mpc_ast_t *layer_ast = mpc_ast_get_child_lb(line_ast, "layer|integer|regex", 0);
                    if (layer_ast)
                        line->setLayer(KicadLayerToEAGLEMapper::mapKicadLayerToEagle(QString(layer_ast->contents).toInt()));

                    package->addWire(line);
                }
                i++;
            }
        }
    }
}

void KicadLegacyFootprintLibImporter::parsePolygonsAstToPackage(mpc_ast_t *polygons_ast, Package *package)
{

}

void KicadLegacyFootprintLibImporter::parsePadsAstToPackage(mpc_ast_t *pads_ast, Package *package)
{
    if (pads_ast) {
        for(int i = 0; i >= 0;) {
            i = mpc_ast_get_index_lb(pads_ast, "pad|>", i);
            if (i >= 0) {
                mpc_ast_t *pad_ast = mpc_ast_get_child_lb(pads_ast, "pad|>", i);
                if (pad_ast) {
                    mpc_ast_t *padname_ast = mpc_ast_get_child_lb(pad_ast, "padname|>", 0);
                    QString name;
                    double w = 0.0, h = 0.0;
                    if (padname_ast) {
                        name = QString(padname_ast->children[2]->children[0]->children[1]->contents);
                        w = m_unitConverter.convertTo_mm(padname_ast->children[4]->children[0]->contents);
                        h = m_unitConverter.convertTo_mm(padname_ast->children[5]->children[0]->contents);
                    }

                    mpc_ast_t *padpos_ast = mpc_ast_get_child_lb(pad_ast, "padpos|>", 0);
                    double x = 0, y = 0;
                    if (padpos_ast) {
                        x = m_unitConverter.convertTo_mm(padpos_ast->children[2]->children[0]->contents);
                        y = -m_unitConverter.convertTo_mm(padpos_ast->children[3]->contents);
                    }

                    mpc_ast_t *padtype_ast = mpc_ast_get_child_lb(pad_ast, "padtype|>", 0);
                    if (padtype_ast && padtype_ast->children_num == 8) {
                        QString padTypeString(padtype_ast->children[2]->contents);
                        if (padTypeString == "SMD" || padTypeString == "CONN") {
                            Smd *smd = new Smd();
                            smd->setX(x);
                            smd->setY(y);
                            smd->setDx(w);
                            smd->setDy(h);
                            smd->setName(name);

                            mpc_ast_t *mask_ast = mpc_ast_get_child_lb(padtype_ast, "mask|hexint|regex", 0);
                            if (mask_ast) {
                                quint16 mask = QString(mask_ast->contents).toInt(nullptr, 16);

                                if (mask & (1<<0)) {
                                    smd->setLayer(EagleLayers::Bottom);
                                    smd->setStop(
                                                (mask & (1<<KicadLayerToEAGLEMapper::BMask))
                                                ? Smd::Stop_yes
                                                : Smd::Stop_no);

                                    smd->setCream(
                                                (mask & (1<<KicadLayerToEAGLEMapper::BPaste))
                                                ? Smd::Cream_yes
                                                : Smd::Cream_no);
                                } else if (mask & (1<<15)) {
                                    smd->setLayer(EagleLayers::Top);
                                    smd->setStop(
                                                (mask & (1<<KicadLayerToEAGLEMapper::FMask))
                                                ? Smd::Stop_yes
                                                : Smd::Stop_no);

                                    smd->setCream(
                                                (mask & (1<<KicadLayerToEAGLEMapper::FPaste))
                                                ? Smd::Cream_yes
                                                : Smd::Cream_no);
                                }
                            }

                            package->addSmd(smd);
                        } else if (padTypeString == "STD") {
                            Pad *pad = new Pad();
                            package->addPad(pad);
                        } else if (padTypeString == "HOLE") {
                            Hole *hole = new Hole();
                            package->addHole(hole);
                        }
                    }
                }
                i++;
            }
        }
    }
}
