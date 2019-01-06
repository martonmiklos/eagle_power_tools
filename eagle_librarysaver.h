#ifndef EAGLE_LIBRARYSAVER_H
#define EAGLE_LIBRARYSAVER_H

#include "qt_eagle_xml_parser/eagle.h"

class EAGLE_LibrarySaver
{
public:
    static bool saveLibrary(Library *lib, const QString & path);
};

#endif // EAGLE_LIBRARYSAVER_H
