#include "eagle_librarysaver.h"

bool EAGLE_LibrarySaver::saveLibrary(Library *lib, const QString &path)
{
    Eagle *eagle = new Eagle();
    bool ok = false;
    eagle = eagle->parseFile(":/resources/blank_lib.lbr", &ok);
    if (ok) {
        eagle->drawing()->setLibrary(lib);
        ok = eagle->writeFile(path);
        delete eagle;
        return ok;
    }
    delete eagle;
    return false;
}
