#include "mainwindow.h"
#include <QApplication>

#include "kicad/kicadlegacyfootprintimport.h"
#include "kicad/kicadlegacysymbolfileimport.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("MM");
    QCoreApplication::setApplicationName("EAGLE power tools");
    QCoreApplication::setApplicationVersion("0.0.1");


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
