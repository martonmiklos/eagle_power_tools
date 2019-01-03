#include "mainwindow.h"
#include <QApplication>

#include "kicad/kicadlegacyfootprintimport.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("MM");
    QCoreApplication::setApplicationName("EAGLE power tools");
    QCoreApplication::setApplicationVersion("0.0.1");

    KicadLegacyFootprintLibImporter importer;
    importer.parseLibrary("/home/mm/AA01B-S040VA1.mod");
    return 0;
    /*QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();*/
}
