#ifndef LIBRARYQACHECKSREGISTRY_H
#define LIBRARYQACHECKSREGISTRY_H

#include "libraryqacheck.h"

#include <QList>

class LibraryQAChecksRegistry
{
public:
    LibraryQAChecksRegistry();

private:
    QList<LibraryQA_Check*> m_checks;
};

#endif // LIBRARYQACHECKSREGISTRY_H
