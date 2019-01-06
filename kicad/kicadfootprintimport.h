#ifndef KICADFOOTPRINTIMPORT_H
#define KICADFOOTPRINTIMPORT_H

#include "kicadimportutils.h"

#include "../qt_eagle_xml_parser/eagle.h"
#include "../sexpresso/sexpresso/sexpresso.hpp"

#include <QString>

class KicadFootprintImport
{
public:
    KicadFootprintImport();

    Library *parseFootprintFile(const QString & libraryPath);
private:
    void parseText(sexpresso::Sexp & text, Package *package);
    void parsePad(sexpresso::Sexp & pad, Package *package);

    void parseSMD_Pad(sexpresso::Sexp & smdPad, Package *package);
    void parseTH_Pad(sexpresso::Sexp & thPad, Package *package);
    void parseHole(sexpresso::Sexp & hole, Package *package);

    void parseLine(sexpresso::Sexp & line, Package *package);
    KicadUnitConverter m_converter;
};

#endif // KICADFOOTPRINTIMPORT_H
