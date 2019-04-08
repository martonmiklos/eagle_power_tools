#include "mainwindow.h"
#include <QApplication>

#include "eagle_librarysaver.h"
#include "kicad/kicadfootprintimport.h"
#include "kicad/kicadlegacyfootprintimport.h"
#include "kicad/kicadlegacysymbolfileimport.h"

#include "lasercut/stencillasercutsvgexport.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("MM");
    QCoreApplication::setApplicationName("EAGLE power tools");
    QCoreApplication::setApplicationVersion("0.0.1");


    /*KicadFootprintImport import1;
    Library *lib = import1.parseFootprintFile("/home/mm/TSSOP-20_4.4x6.5mm_Pitch0.65mm.kicad_mod");

    //return 0;

    KicadLegacyFootprintLibImporter importer2;
    Library *lib = importer2.parseModFile("/home/mm/AA01B-S040VA1.mod");

    KicadLegacySymbolFileImport importer;
    importer.parseSymbolLibFile("/home/mm/symbol.lib", lib);
    EAGLE_LibrarySaver::saveLibrary(lib, "/tmp/foo.lbr");
    return 0;*/

    /*bool ok = false;
    Eagle *brd = Eagle::parseFile("/home/mm/Projektek/eagle/ble_terrain_twister/ble_terrain_twister.brd", &ok);
    StencilLaserCutSVGExport se;
    se.generateSVG(EagleLayers::bCream, brd->drawing()->board(), "", "/tmp/foo.svg");
    return 0;*/
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
