#include "mainwindow.h"
#include <QApplication>

#include "eagle_librarysaver.h"
#include "kicad/kicadfootprintimport.h"
#include "kicad/kicadlegacyfootprintimport.h"
#include "kicad/kicadlegacysymbolfileimport.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("MM");
    QCoreApplication::setApplicationName("EAGLE power tools");
    QCoreApplication::setApplicationVersion("0.0.1");


    KicadFootprintImport import1;
    EAGLE_LibrarySaver::saveLibrary(import1.parseFootprintFile("/home/mm/TSSOP-20_4.4x6.5mm_Pitch0.65mm.kicad_mod"), "/tmp/foo.lbr");
    return 0;

    KicadLegacyFootprintLibImporter importer2;
    Library *lib = importer2.parseModFile("/home/mm/AA01B-S040VA1.mod");

    KicadLegacySymbolFileImport importer;
    importer.parseSymbolLibFile("/home/mm/conn-x220-kb.lib", lib);
    importer2.save();
    return 0;
    /*QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();*/
}
